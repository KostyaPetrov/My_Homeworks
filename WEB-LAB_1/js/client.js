var form= document.querySelector('.validate_form');
var validatedButton = document.querySelector('.validate_button');
var xOptions = document.querySelectorAll(".x");
var yCoordinate = document.querySelector(".y");
var rCoordinate = document.querySelector(".R");


function isNumber(s) {
  var n = parseFloat(s.replace(',', '.'));
  return !isNaN(n) && isFinite(n);
}

//функция для генерации ошибок
function generateTip(text, color) {
  var tip = document.createElement('div');
  tip.className = 'tip';
  tip.style.color = color;
  tip.innerHTML = text;
  return tip;
}


//функция для очистки подсказок при повторной валидации 
function removeValidation() {
  var tips = form.querySelectorAll('.tip')
  for (var i = 0; i < tips.length; i++) {
    tips[i].remove()
  }
}

// проверка значения в поле на попадание в заданный диапазон
function validateField(coordinate, min, max) {
  if (coordinate.value) {
    coordinate.value = coordinate.value.replace(',', '.');
    if (coordinate.value < min || coordinate.value > max || !isNumber(coordinate.value)) {
      var error = generateTip('Wrong number format', 'red')
      coordinate.parentElement.insertBefore(error, coordinate)
      return false;
    }
    else {
      var correct = generateTip('Correct data', 'green');
      coordinate.parentElement.insertBefore(correct, coordinate)
      return true;
    }
  }
  var error = generateTip('field is blank', 'red');
  coordinate.parentElement.insertBefore(error, coordinate);
  return false;
}


// фунция для повторной проверки, что поля заполнены верно, чтобы передать их php скрипту
function validateAll() {
  return validateField(yCoordinate, -5, 3) && validateField(rCoordinate, 1, 5);
}

$(document).ready(function () {

  $.ajax({
    url: 'php/load.php',
    method: "GET",
    dataType: "html",
    success: function (data) {
      console.log(data);
      $("#content").html(data);
    },
    error: function (error) {
      console.log(error);
    },
  })
})

$("#validate_form").on("submit", function (event) {
  event.preventDefault();

  console.log("Got data for check!");
  console.log('y: ', yCoordinate.value);
  console.log('R: ', rCoordinate.value);
  removeValidation();

  if (!validateAll()) {
    console.log("get canceled")
    return
  }
  console.log("data sending...")
  console.log($(this).serialize());
  $.ajax({
    url: 'php/server.php',
    method: "GET",
    data: $(this).serialize() + "&timezone=" + new Date().getTimezoneOffset(),
    dataType: "html",

    success: function (data) {
      console.log(data);
      $(".validate_button").attr("disabled", false);
      $("#content").html(data);
    },
    error: function (error) {
      console.log(error);
      $(".validate_button").attr("disabled", false);
    },
  })
});

$(".reset_button").on("click", function () {
  $.ajax({
    url: 'php/clear.php',
    method: "GET",
    dataType: "html",
    success: function (data) {
      console.log(data);
      $("#content").html(data);
    },
    error: function (error) {
      console.log(error);
    },
  })
})
$("input:checkbox").on('click', function() {
  // in the handler, 'this' refers to the box clicked on
  var $box = $(this);
  if ($box.is(":checked")) {
    // the name of the box is retrieved using the .attr() method
    // as it is assumed and expected to be immutable
    var group = "input:checkbox[name='" + $box.attr("name") + "']";
    // the checked state of the group/box on the other hand will change
    // and the current value is retrieved using .prop() method
    $(group).prop("checked", false);
    $box.prop("checked", true);
  } else {
    $box.prop("checked", false);
  }
});