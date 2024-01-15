package calc.controllers;

import calc.dao.HistoryDAO;
import calc.models.CalcModel;
import calc.models.History;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;


import java.sql.SQLException;


@Controller
public class MainController {

    private final HistoryDAO historyDAO;

    @Autowired
    public MainController(HistoryDAO historyDAO) {
        this.historyDAO = historyDAO;
    }

    @GetMapping("/")
    public String index(Model model) throws SQLException {
        model.addAttribute("calc", new CalcModel());
        model.addAttribute("history", historyDAO.index());
        return "index";
    }

    @PostMapping("/")
    public String calcResult(@ModelAttribute("calc") CalcModel calcModel, Model model) throws SQLException {
        historyDAO.save(new History(calcModel.getExpression(), calcModel.getExpressionX()));
        calcModel.calcResult();
        calcModel.calcGraphData();
        model.addAttribute("calc", calcModel);
        model.addAttribute("history", historyDAO.index());
        return "index";
    }

    @GetMapping("about")
    public String about(Model model) {
        return "about";
    }

    @GetMapping("clear_history")
    public String clearHistory(Model model) throws SQLException {
        historyDAO.clear();
        model.addAttribute("calc", new CalcModel());
        model.addAttribute("history", historyDAO.index());
        return "redirect:/";
    }

    @GetMapping("/*")
    public String handle() {
        return "404page";
    }
}
