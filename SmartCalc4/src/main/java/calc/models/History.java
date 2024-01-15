package calc.models;

public class History {
    private String expression;
    private String expressionX;

    public History() {
        expression = "";
        expressionX = "";
    }

    public History(String expression, String expressionX) {
        this.expression = expression;
        this.expressionX = expressionX;
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
}
