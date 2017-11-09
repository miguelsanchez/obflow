import java.io.IOException;
import java.io.OutputStream;

/**
 * The YAMLGenerator closes the output stream on writeValue()..
 *
 */
public class NonCloseableOutputStream extends OutputStream {
    private final OutputStream wrappedOutputStream;
    public NonCloseableOutputStream(final OutputStream wrappedOutputStream) {
        this.wrappedOutputStream = wrappedOutputStream;
    }

    @Override
    public void write(int b) throws IOException {
        wrappedOutputStream.write(b);
    }

    @Override
    public void close() throws IOException {
        //super.close();
    }
}
