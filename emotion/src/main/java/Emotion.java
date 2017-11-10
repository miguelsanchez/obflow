import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializationFeature;
import com.fasterxml.jackson.dataformat.yaml.YAMLFactory;
import com.squareup.okhttp.*;

import java.io.*;
import java.util.*;

public class Emotion {
    private final OkHttpClient httpClient = new OkHttpClient();
    private final ObjectMapper jsonMapper;
    private final ObjectMapper yamlMapper;
    private final OutputStream outputStream;
    private final NonCloseableOutputStream nonCloseableOutputStream;
    private boolean run = true;
    private String username;

    public static void main(String[] args) throws IOException {
        if (args.length >= 2) {
            System.out.println("Reading images from dir " + args[0] + " and writing YAML proteins to " + args[1]);
            final File imagesDir = new File(args[0]);
            if (!imagesDir.isDirectory()) {
                System.err.println(args[0] + " is not a directory.");
                return;
            }
            if (!imagesDir.canRead()) {
                System.err.println(args[0] + " is not readable.");
                return;
            }
            if (!imagesDir.canWrite()) {
                System.err.println(args[0] + " is not writable.");
                return;
            }
            final File proteinPipeFile = new File(args[1]);
            if (!proteinPipeFile.canWrite()) {
                System.err.println(args[1] + " is not writable.");
                return;
            }
            final FileOutputStream proteinPipeOutputStream = new FileOutputStream(proteinPipeFile);
            final Emotion emotion = new Emotion(proteinPipeOutputStream);
            if (args.length == 3) {
                emotion.setUsername(args[2]);
            }
            emotion.monitorDirectory(imagesDir);
        } else {
            System.err.println("Give images directory to monitor and output file(or named pipe) for Greenhouse YAML proteins as args");
        }
    }

    public Emotion(final OutputStream outputStream) {
        jsonMapper = new ObjectMapper();
        jsonMapper.enable(SerializationFeature.INDENT_OUTPUT);
        YAMLFactory yamlFactory = new YAMLFactory();
        yamlFactory.disable(JsonGenerator.Feature.AUTO_CLOSE_TARGET);
        yamlMapper = new ObjectMapper(yamlFactory);
        this.outputStream = outputStream;
        nonCloseableOutputStream = new NonCloseableOutputStream(outputStream);
    }

    public void monitorDirectory(final File dir) throws IOException {
        final File[] oldfiles = dir.listFiles();
        if (oldfiles.length > 0) {
            System.out.println("Deleting old files...");
            for (final File file : oldfiles) {
                file.delete();
            }
        }
        while (run) {
            for (final File file : dir.listFiles()) {
                String filename = file.getName();
                if (!filename.endsWith(".jpeg")) {
                    continue;
                }

                final int length = (int) file.length();
                final byte[] imageByteBuffer = new byte[length];
                final InputStream in = new FileInputStream(file);
                int imageByteCount = 0;
                int b;
                while ((b = in.read()) != -1) {
                    imageByteBuffer[imageByteCount++] = (byte) (0xFF & b);
                }
                in.close();
                file.delete();
                postImage(filename, imageByteBuffer, imageByteCount);
            }
        }

        httpClient.getDispatcher().getExecutorService().shutdown();
    }

//    private void processImage(String[] args) throws IOException {
//        final InputStream in;
//        final byte[] imageByteBuffer;
//        if (args.length == 1) {
//            System.out.println("Reading image from file " + args[0]);
//            final File imagefile = new File(args[0]);
//            final int length = (int) imagefile.length();
//            imageByteBuffer = new byte[length];
//            in = new FileInputStream(imagefile);
//        } else {
//            System.out.println("Reading image from stdin..");
//            imageByteBuffer  = new byte[1024 * 128];
//            in = System.in;
//        }
//
//        int imageByteCount = 0;
//        int b;
//        while ((b = in.read()) != -1) {
//            imageByteBuffer[imageByteCount++] = (byte)(0xFF & b);
//        }
//        System.out.println("Read " + imageByteCount + " bytes.");
//        postImage(imageByteBuffer, imageByteCount);
//    }

    private int postImageCallCounter;

    public void postImage(final String tag, final byte[] imageByteBuffer, final int imageLength) {
        if (postImageCallCounter++ % 16 != 0) {
            // Only send 1 frame every 3 seconds.
            return;
        }
        System.out.println("Sending:" + tag);
        final Request request = new Request.Builder().tag(tag)
                .url("https://westus.api.cognitive.microsoft.com/emotion/v1.0/recognize")
                .addHeader("Content-Type", "application/octet-stream")
                .addHeader("Ocp-Apim-Subscription-Key", "35fe1a6f121d483a9a4b72d1ce6468c0")
                .post(RequestBody.create(MediaType.parse("application/octet-stream"), imageByteBuffer, 0, imageLength))
                .build();

        final long startTime = System.currentTimeMillis();

        httpClient.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Request request, IOException e) {
                System.out.println("Failure for tag:" + request.tag());
                e.printStackTrace();
            }

            @Override
            public void onResponse(Response response) throws IOException {
                try (ResponseBody responseBody = response.body()) {
                    if (!response.isSuccessful()) throw new IOException("Unexpected code " + response);
                    final long endTime = System.currentTimeMillis();
                    final List<Face> faces = jsonMapper.readValue(responseBody.byteStream(), new TypeReference<List<Face>>() {
                    });
                    System.out.println("Success of tag:" + tag);

                    //System.out.println(jsonMapper.writeValueAsString(faces));

                    final ResultProtein resultProtein = new ResultProtein();
                    if (username == null) {
                        int faceIndex = 0;
                        final Scores avgScores = new Scores();
                        for (final Face face : faces) {
                            resultProtein.addScores("face" + faceIndex++, face.scores);
                            avgScores.add(face.scores);
                        }
                        if (faceIndex > 0) {
                            avgScores.avg(faceIndex);
                            resultProtein.addScores("face-avg", avgScores);
                        }
                    } else if (!faces.isEmpty()){
                        resultProtein.addScores(username, faces.get(0).scores);
                    }
                    yamlMapper.writeValue(nonCloseableOutputStream, resultProtein);
                }
            }
        });
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public static class FaceRectangle {
        public int width;
        public int height;
        public int left;
        public int top;
    }

    public static class Scores {
        public float anger;
        public float contempt;
        public float disgust;
        public float fear;
        public float happiness;
        public float neutral;
        public float sadness;
        public float surprise;

        public void add(Scores other) {
            anger += other.anger;
            contempt += other.contempt;
            disgust += other.disgust;
            fear += other.fear;
            happiness += other.happiness;
            neutral += other.neutral;
            sadness += other.sadness;
            surprise += other.surprise;
        }

        public void avg(final int count) {
            anger /= count;
            contempt /= count;
            disgust /= count;
            fear /= count;
            happiness /= count;
            neutral /= count;
            sadness /= count;
            surprise /= count;
        }
    }

    public static class Face {
        public FaceRectangle faceRectangle;
        public Scores scores;
    }

    public static class ResultProtein {
        public List<String> descrips = Arrays.asList(new String[]{"greenhouse", "ms-emotion"});
        public Map<String, Object> ingests = new HashMap();

        public void addScores(final String key, final Scores scores) {
            ingests.put(key, scores);
        }
    }
}
