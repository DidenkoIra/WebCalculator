package calc.models;

public class CalcModel {
    private String expression;
    private String expressionX;
    private int xBegin;
    private int xEnd;
    private int yBegin;
    private int yEnd;
    private double[][] graphData;
    private String result;
    public CalcModel() {
        expression = "";
        expressionX = "";
        result = "";
        xBegin = -5;
        xEnd = 5;
        yBegin = -5;
        yEnd = 5;
        graphData = new double[1][1];
        graphData[0][0] = 0;
    }
    public CalcModel(String expression, String expressionX) {
        this.expression = expression;
        if (expressionX == null) {
            this.expressionX = "";
        } else  {
            this.expressionX = expressionX;
        }
        result = "";
    }

    public String getExpression() {
        return expression;
    }

    public void setExpression(String expression) {
        this.expression = expression;
    }

    public String getExpressionX() {
        return expressionX;
    }

    public void setExpressionX(String expressionX) {
        this.expressionX = expressionX;
    }

    public int getXBegin() {
        return xBegin;
    }
    public void setXBegin(int xBegin) {
        this.xBegin = xBegin;
    }

    public int getXEnd() {
        return xEnd;
    }

    public void setXEnd(int xEnd) {
        this.xEnd = xEnd;
    }

    public int getYBegin() {
        return yBegin;
    }

    public void setYBegin(int yBegin) {
        this.yBegin = yBegin;
    }

    public int getYEnd() {
        return yEnd;
    }

    public void setYEnd(int yEnd) {
        this.yEnd = yEnd;
    }

    public String getResult() {
        return result;
    }

    public double[][] getGraphData() {
        return graphData;
    }

    public void calcResult() {
        CalcModelJNI calcModelJNI = new CalcModelJNI();
        result = calcModelJNI.calcResult(expression, expressionX);
    }

    public void calcGraphData() {
        CalcModelJNI calcModelJNI = new CalcModelJNI();
        graphData = calcModelJNI.calcGraphData(expression, xBegin, xEnd);
    }

    public String getResultView() {
        if(result.isEmpty()) {
            return "Let's start!";
        } else if (result.equals("Error!")) {
            return "ERROR!";
        } else {
            StringBuilder stringBuilder = new StringBuilder(expression);
            stringBuilder.append(" = ").append(result);
            if (expression.toLowerCase().contains("x")) {
                stringBuilder.append("\tx = ").append(expressionX);
            }
            return stringBuilder.toString();
        }
    }
}
