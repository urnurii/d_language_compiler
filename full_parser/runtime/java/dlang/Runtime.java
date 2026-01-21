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

    public static String readln() {
        try {
            String line = READER.readLine();
            return line == null ? "" : line;
        } catch (IOException ex) {
            return "";
        }
    }

    public static int readf(String fmt, Object... args) {
        if (fmt == null || args == null) {
            return 0;
        }
        int argIndex = 0;
        int assigned = 0;
        for (int i = 0; i < fmt.length(); i++) {
            char c = fmt.charAt(i);
            if (c != '%') {
                continue;
            }
            if (i + 1 >= fmt.length()) {
                break;
            }
            char spec = fmt.charAt(i + 1);
            i++;
            if (spec == '%') {
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
                        int val = Integer.parseInt(tok);
                        if (arg instanceof int[]) {
                            int[] a = (int[]) arg;
                            if (a.length > 0) {
                                a[0] = val;
                                assigned++;
                            }
                        }
                    } catch (Exception ex) {
                        return assigned;
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
}
