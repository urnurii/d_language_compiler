package dlang;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;

public final class Runtime {
    private static final BufferedReader READER = new BufferedReader(new InputStreamReader(System.in));
    private static final Scanner SCANNER = new Scanner(System.in);

    private Runtime() {
    }

    private static String normalizeFormat(String fmt) {
        if (fmt == null) {
            return null;
        }
        StringBuilder out = new StringBuilder(fmt.length() + 8);
        int i = 0;
        while (i < fmt.length()) {
            char c = fmt.charAt(i);
            if (c != '%') {
                out.append(c);
                i++;
                continue;
            }
            if (i + 1 < fmt.length() && fmt.charAt(i + 1) == '%') {
                out.append("%%");
                i += 2;
                continue;
            }
            out.append('%');
            i++;
            int segStart = out.length();
            while (i < fmt.length()) {
                char f = fmt.charAt(i);
                if (f == '-' || f == '+' || f == ' ' || f == '0' || f == '#') {
                    out.append(f);
                    i++;
                } else {
                    break;
                }
            }
            while (i < fmt.length()) {
                char d = fmt.charAt(i);
                if (d >= '0' && d <= '9') {
                    out.append(d);
                    i++;
                } else {
                    break;
                }
            }
            boolean hadPrecision = false;
            int precisionStart = out.length();
            if (i < fmt.length() && fmt.charAt(i) == '.') {
                hadPrecision = true;
                out.append('.');
                i++;
                if (i < fmt.length()) {
                    char d = fmt.charAt(i);
                    if (d < '0' || d > '9') {
                        out.append('0');
                    }
                } else {
                    out.append('0');
                }
                while (i < fmt.length()) {
                    char d = fmt.charAt(i);
                    if (d >= '0' && d <= '9') {
                        out.append(d);
                        i++;
                    } else {
                        break;
                    }
                }
            }
            if (i < fmt.length()) {
                char spec = fmt.charAt(i);
                boolean isFloatSpec = spec == 'f' || spec == 'F' || spec == 'e' || spec == 'E' ||
                                      spec == 'g' || spec == 'G';
                if (!isFloatSpec && hadPrecision) {
                    out.delete(precisionStart, out.length());
                }
                out.append(spec);
                i++;
            } else {
                if (hadPrecision) {
                    out.delete(precisionStart, out.length());
                }
            }
        }
        return out.toString();
    }

    private static int scanFormatSpecifier(String fmt, int index, char[] outSpec) {
        int i = index + 1;
        if (i >= fmt.length()) {
            return -1;
        }
        if (fmt.charAt(i) == '%') {
            return i + 1;
        }
        while (i < fmt.length()) {
            char f = fmt.charAt(i);
            if (f == '-' || f == '+' || f == ' ' || f == '0' || f == '#') {
                i++;
            } else {
                break;
            }
        }
        while (i < fmt.length()) {
            char d = fmt.charAt(i);
            if (d >= '0' && d <= '9') {
                i++;
            } else {
                break;
            }
        }
        if (i < fmt.length() && fmt.charAt(i) == '.') {
            i++;
            while (i < fmt.length()) {
                char d = fmt.charAt(i);
                if (d >= '0' && d <= '9') {
                    i++;
                } else {
                    break;
                }
            }
        }
        if (i >= fmt.length()) {
            return -1;
        }
        if (outSpec != null && outSpec.length > 0) {
            outSpec[0] = fmt.charAt(i);
        }
        return i + 1;
    }

    public static String readln() {
        try {
            String line = READER.readLine();
            return line == null ? "" : line;
        } catch (IOException ex) {
            return "";
        }
    }

    private static RuntimeException sliceError(String msg) {
        return new RuntimeException(msg);
    }

    private static Object newEmptyArrayByTag(int typeTag, String className) {
        switch (typeTag) {
            case 1:
                return new boolean[0];
            case 2:
                return new int[0];
            case 3:
                return new float[0];
            case 4:
                return new double[0];
            case 5:
                return new String[0];
            case 6: {
                if (className == null) {
                    return new Object[0];
                }
                try {
                    String binary = className.replace('/', '.');
                    Class<?> cls = Class.forName(binary);
                    return java.lang.reflect.Array.newInstance(cls, 0);
                } catch (ClassNotFoundException ex) {
                    throw sliceError("Slice error: class not found: " + className);
                }
            }
            case 7:
                return new int[0];
            default:
                return new Object[0];
        }
    }

    public static Object __slice(Object arr, int start, int end) {
        if (arr == null) {
            if (start == 0 && end == 0) {
                return null;
            }
            throw sliceError("Slice error: null array");
        }
        Class<?> cls = arr.getClass();
        if (!cls.isArray()) {
            throw sliceError("Slice error: not an array");
        }
        int len = java.lang.reflect.Array.getLength(arr);
        if (start < 0 || end < 0 || start > end || end > len) {
            throw sliceError("Slice error: bounds start=" + start + " end=" + end + " len=" + len);
        }
        int outLen = end - start;
        Object out = java.lang.reflect.Array.newInstance(cls.getComponentType(), outLen);
        if (outLen > 0) {
            System.arraycopy(arr, start, out, 0, outLen);
        }
        return out;
    }

    public static Object __slice_typed(Object arr, int start, int end, int typeTag, String className) {
        if (arr == null) {
            if (start == 0 && end == 0) {
                return newEmptyArrayByTag(typeTag, className);
            }
            throw sliceError("Slice error: null array");
        }
        Class<?> cls = arr.getClass();
        if (!cls.isArray()) {
            throw sliceError("Slice error: not an array");
        }
        int len = java.lang.reflect.Array.getLength(arr);
        if (start < 0 || end < 0 || start > end || end > len) {
            throw sliceError("Slice error: bounds start=" + start + " end=" + end + " len=" + len);
        }
        int outLen = end - start;
        Object out = java.lang.reflect.Array.newInstance(cls.getComponentType(), outLen);
        if (outLen > 0) {
            System.arraycopy(arr, start, out, 0, outLen);
        }
        return out;
    }

    public static void __slice_assign(Object arr, int start, int end, Object rhs) {
        if (arr == null) {
            throw sliceError("Slice assign error: null array");
        }
        if (rhs == null) {
            throw sliceError("Slice assign error: null rhs");
        }
        Class<?> cls = arr.getClass();
        if (!cls.isArray() || !rhs.getClass().isArray()) {
            throw sliceError("Slice assign error: not an array");
        }
        int len = java.lang.reflect.Array.getLength(arr);
        if (start < 0 || end < 0 || start > end || end > len) {
            throw sliceError("Slice assign error: bounds start=" + start + " end=" + end + " len=" + len);
        }
        if (rhs.getClass() != arr.getClass()) {
            throw sliceError("Slice assign error: incompatible array types");
        }
        int rhsLen = java.lang.reflect.Array.getLength(rhs);
        int outLen = end - start;
        if (rhsLen != outLen) {
            throw sliceError("Slice assign error: length mismatch");
        }
        if (outLen > 0) {
            System.arraycopy(rhs, 0, arr, start, outLen);
        }
    }

    public static int readf(String fmt, Object... args) {
        if (fmt == null || args == null) {
            return 0;
        }
        fmt = normalizeFormat(fmt);
        int argIndex = 0;
        int assigned = 0;
        char[] specBuf = new char[1];
        for (int i = 0; i < fmt.length(); ) {
            if (fmt.charAt(i) != '%') {
                i++;
                continue;
            }
            int next = scanFormatSpecifier(fmt, i, specBuf);
            if (next < 0) {
                break;
            }
            i = next;
            char spec = specBuf[0];
            if (spec == 0) {
                continue;
            }
            if (argIndex >= args.length) {
                break;
            }
            Object arg = args[argIndex++];
            if (!SCANNER.hasNext()) {
                break;
            }
            switch (spec) {
                case 'd': {
                    String tok = SCANNER.next();
                    try {
                        double num = Double.parseDouble(tok);
                        int val = (int) num;
                        if (arg instanceof int[]) {
                            int[] a = (int[]) arg;
                            if (a.length > 0) {
                                a[0] = val;
                                assigned++;
                            }
                        }
                    } catch (Exception ex) {
                        throw new RuntimeException("Invalid integer input: " + tok);
                    }
                    break;
                }
                case 'f': {
                    String tok = SCANNER.next();
                    try {
                        double val = Double.parseDouble(tok);
                        if (arg instanceof double[]) {
                            double[] a = (double[]) arg;
                            if (a.length > 0) {
                                a[0] = val;
                                assigned++;
                            }
                        } else if (arg instanceof float[]) {
                            float[] a = (float[]) arg;
                            if (a.length > 0) {
                                a[0] = (float) val;
                                assigned++;
                            }
                        }
                    } catch (Exception ex) {
                        return assigned;
                    }
                    break;
                }
                case 's': {
                    String val = SCANNER.next();
                    if (arg instanceof String[]) {
                        String[] a = (String[]) arg;
                        if (a.length > 0) {
                            a[0] = val;
                            assigned++;
                        }
                    }
                    break;
                }
                case 'c': {
                    String tok = SCANNER.next();
                    char ch = tok.isEmpty() ? 0 : tok.charAt(0);
                    if (arg instanceof int[]) {
                        int[] a = (int[]) arg;
                        if (a.length > 0) {
                            a[0] = (int) ch;
                            assigned++;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
        return assigned;
    }

    public static void writef(String fmt, Object... args) {
        if (fmt == null) {
            return;
        }
        fmt = normalizeFormat(fmt);
        try {
            System.out.printf(fmt, args);
        } catch (Exception ex) {
            System.out.print(fmt);
        }
    }

    public static String format(String fmt, Object... args) {
        if (fmt == null) {
            return "";
        }
        fmt = normalizeFormat(fmt);
        try {
            return String.format(fmt, args);
        } catch (Exception ex) {
            return fmt;
        }
    }
}
