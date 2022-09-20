<?php

function validate_number($val, $min, $max){
    return isset($val) && is_numeric($val) && ($val >= $min) && ($val <= $max);
}

function validate_timezone($timezone){
    return isset($timezone);
}

//Проверка на попадание в необходимую область

function check_first_area($x, $y, $r){
    return($x<0 && $y>0 && (($x/($r/2)) * $y) >= $y); // дописать попадание точки в треугольник
}

function check_second_area($x, $y, $r){
    return($x>=0 && $y >= 0 && $x <= $r && $y <= ($r/2));//квадрат
}

function check_third_area($x, $y, $r){
    return($y <= 0 && $x >= 0 && ($x*$x + $y*$y) <= (($r/2)*($r/2)));//четверть сферы
}

session_start();

if (!isset($_SESSION['data'])){
    $_SESSION['data'] = array();
}

$x = @$_GET["x"];
$y = @$_GET["y"];
$r = @$_GET["r"];
$timezone= @$_GET["timezone"];

if(validate_number($x,-3,5) && validate_number($y,-5,3) && validate_number($r,1,5) && validate_timezone($timezone)){
    $is_inside = check_first_area($x, $y, $r) || check_second_area($x, $y, $r) || check_third_area($x, $y, $r);
	$hit_fact = $is_inside ? "Hit": "Miss";
	$current_time = date("j M o G:i:s", time()-$timezone*60);
	$execution_time = round(microtime(true) - $_SERVER['REQUEST_TIME_FLOAT'], 7);
	
	$answer = array("x"=>$x, "y"=>$y, "r"=>$r, "hit_fact"=>$hit_fact, "current_time"=>$current_time, "execution_time"=>$execution_time);
	array_push($_SESSION['data'], $answer);
}

foreach ($_SESSION['data'] as $elem){
	echo "<div class='results'>";
	echo "<div class='table_column' id='column_x'>". $elem['x']. "</div>";
	echo "<div class='table_column' id='column_y'>". $elem['y'] ."</div>";
	echo "<div class='table_column' id='column_R'>". $elem['r']."</div>";
	echo "<div class='table_column' id='column_hit'>". $elem['hit_fact']  . "</div>";
	echo "<div class='table_column' id='column_time'>". $elem['current_time']  ."</div>";
	echo "<div class='table_column' id='run_time'>". $elem['execution_time'] ."</div>";
	echo "</div>";
}
