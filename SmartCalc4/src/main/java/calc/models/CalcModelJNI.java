package calc.models;

public class CalcModelJNI {
    static {
        System.loadLibrary("smartcalc");
    }
    public CalcModelJNI() {}
    native String calcResult(String expression, String expressionX);
    native double[][] calcGraphData(String expression, double xBegin, double xEnd);
}
