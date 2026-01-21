package compilator.fix;

import org.objectweb.asm.*;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.objectweb.asm.ClassWriter.COMPUTE_FRAMES;
import static org.objectweb.asm.ClassWriter.COMPUTE_MAXS;

/**
 * Utility for post-processing JVM .class files using ASM.
 *
 * <p>
 * This tool reads a compiled {@code .class} file and rewrites it using
 * {@link org.objectweb.asm.ClassWriter} with {@code COMPUTE_MAXS} and
 * {@code COMPUTE_FRAMES} flags enabled.
 * </p>
 *
 * <p>
 * It is intended for scenarios where a {@code .class} file was generated
 * manually or partially (e.g. by a custom bytecode generator),
 * and some mandatory metadata is missing or incorrect:
 * </p>
 *
 * <ul>
 *   <li>{@code max_stack}</li>
 *   <li>{@code max_locals}</li>
 *   <li>{@code StackMapTable} frames</li>
 * </ul>
 *
 * <p>
 * ASM automatically analyzes the bytecode and recomputes all required
 * stack sizes and stack map frames, producing a fully valid JVM class file
 * that can be verified and executed by the JVM.
 * </p>
 *
 * <p><b>Typical use case:</b></p>
 * <pre>
 * 1. Generate a .class file with placeholders for max_stack / frames
 * 2. Run this tool on the generated file
 * 3. Obtain a valid, JVM-verifiable .class file
 * </pre>
 */
public class FixClass {
    public static byte[] fix(byte[] input) {
        ClassReader cr = new ClassReader(input);

        ClassWriter cw = new ClassWriter(ClassWriter.COMPUTE_FRAMES | ClassWriter.COMPUTE_MAXS);

        cr.accept(cw, ClassReader.EXPAND_FRAMES);

        return cw.toByteArray();
    }

    public static void main(String[] args) throws Exception {
            if (args.length != 2) {
                System.err.println("Usage: FixClass <input> <output>");
                System.exit(2);
            }

            Path in  = Path.of(args[0]);
            Path out = Path.of(args[1]);

            byte[] inputBytes = Files.readAllBytes(in);
            byte[] fixedBytes = fix(inputBytes);
            Files.write(out, fixedBytes);
        }
}
