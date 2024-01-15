package calc.models;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvFileSource;
import org.junit.jupiter.params.provider.ValueSource;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class CalcModelTest {
    @ParameterizedTest
    @CsvFileSource(resources = "/test_data.csv")
    void calcResultExpression(String expression, String result) {
        String x = "";
        CalcModel calcModel = new CalcModel(expression, null);
        calcModel.calcResult();
        assertEquals(calcModel.getResult(), result);
    }

    @ParameterizedTest
    @CsvFileSource(resources = "/test_data_withx.csv")
    void calcResultExpressionWithX(String expression, String x, String result) {
        CalcModel calcModel = new CalcModel(expression, x);
        calcModel.calcResult();
        assertEquals(calcModel.getResult(), result);
    }

    @ParameterizedTest
    @ValueSource(strings = {"...", "5//9", "sin(", "2x", "a"})
    void calcResultError(String expression) {
        String x = "";
        String result = "Error!";
        CalcModel calcModel = new CalcModel(expression, x);
        calcModel.calcResult();
        assertEquals(calcModel.getResult(), result);
    }

    @ParameterizedTest
    @ValueSource(strings = {"...", "5//9", "sin(", "2x", "a"})
    void calcResultXError(String x) {
        String expression = "x";
        String result = "Error!";
        CalcModel calcModel = new CalcModel(expression, x);
        calcModel.calcResult();
        assertEquals(calcModel.getResult(), result);
    }

    @Test
    void calcGraphData() {
        CalcModel calcModel = new CalcModel("x", null);
        calcModel.calcGraphData();
        double[][] result = calcModel.getGraphData();
        assertArrayEquals(result[0], result[1]);
    }

}
