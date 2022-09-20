<?php
session_set_cookie_params(0);
session_start();
if(isset($_SESSION['data'])){
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
}
