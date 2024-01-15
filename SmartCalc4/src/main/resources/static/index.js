var idOfInputFocused = "expression";

function onFocus(input) {
  idOfInputFocused = input.id;
}

function onButtonClick() {
  document.getElementById(idOfInputFocused).value = document.getElementById(idOfInputFocused).value
                                                  + event.target.textContent;
}

function onButtonFuncClick() {
  document.getElementById(idOfInputFocused).value = document.getElementById(idOfInputFocused).value
                                                  + event.target.textContent + "(";
}

function onButtonACClick() {
  document.getElementById("expression").value = "";
  document.getElementById("expressionX").value = "";
}

function onButtonHistoryClick() {
  var historyItem = event.target.textContent;
  var arrayOfStrings = historyItem.split(", x = ");
  document.getElementById("expression").value = arrayOfStrings[0];
  document.getElementById("expressionX").value = arrayOfStrings[1];
}
