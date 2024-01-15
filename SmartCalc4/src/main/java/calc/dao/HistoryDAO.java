package calc.dao;

import calc.models.History;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import javax.annotation.PreDestroy;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

@Component
public class HistoryDAO {
    private final Connection connection;

    @Autowired
    public HistoryDAO(Connection connection) {
        this.connection = connection;
    }

    public List<History> index() throws  SQLException{
        List<History> list = new ArrayList<>();
        String selectDataQuery = "SELECT * FROM history";
        Statement statement = connection.createStatement();

        ResultSet resultSet = statement.executeQuery(selectDataQuery);

        while (resultSet.next()) {
            list.add(new History(resultSet.getString("expression"),
                    resultSet.getString("expressionX")));
        }
        statement.close();
        return list;
    }

    public void save(History history) throws SQLException {
        String insertDataQuery = String.format("INSERT INTO history VALUES ('%s', '%s')",
                history.getExpression(), history.getExpressionX());
        Statement statement = connection.createStatement();
        statement.execute(insertDataQuery);
        statement.close();
    }

    public void clear() throws SQLException {
        String deleteDataQuery = "DELETE FROM history";
        Statement statement = connection.createStatement();
        statement.execute(deleteDataQuery);
        statement.close();
    }

    @PreDestroy
    public void closeConnection() {
        try {
            connection.close();
        } catch (SQLException sqlException) {

        }
    }
}
