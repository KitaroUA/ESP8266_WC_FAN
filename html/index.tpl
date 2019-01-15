<!DOCTYPE html>

<html>
<head>

<title>IoT WC FAN</title>



<script src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
<script>    window.jQuery || document.write('<script src="../js/jquery-3.2.1.min.js"><\/script>')</script>
<link rel="stylesheet" type="text/css" href="style.css">




<script type="text/javascript" src="index_1.js"></script>
<script type="text/javascript" src="index_2.js"></script>

<meta charset="UTF-8">
</head>

<script type="text/javascript">
function get_data(){	
$.get("index_get_data.cgi" , {sensor_data:"get_data"}, function( data ) {
//	$("#working_time").html(data);
//	$("#current_time").load(window.location.href + " #temperature");




		var myVars;
		var line=data.split(';');
		myVars={
        'cur_time':				line[0],
        'date':					line[1],
        'temper_DHT':			line[2],
        'hum_DHT':				line[3],
        'checker_1_state':		line[4],
        'checker_1_time':		line[5],
        'uptime':				line[6],
        'try':					line[7],
        'indicator_color':		line[8],
        }
 

		$("#current_time").html("Поточний час: "+myVars.cur_time+"<br>"+myVars.date);
		$("#BME").html("Температура: "+myVars.temper_DHT+"°С, вологість: "+myVars.hum_DHT+"&#37");
		$("#myCanvas").attr("data-color" , myVars.indicator_color);
		draw_color_circle();
		if (myVars.checker_1_state === "1")
			{
			$( "#slider1" ).prop( "checked", true );
			$("#checker_1_text").html("Вентилятор тимчасово увімкнено ще на: "+myVars.checker_1_time +" хв.");
			}
			else
			{
			$( "#slider1" ).prop( "checked", false );
			$("#checker_1_text").html("Тимчасово увімкнути вентилятор на: "+myVars.checker_1_time+" хв.");
			}
		$("#onoffswitch1").load(window.location.href + "#onoffswitch1");	

		$("#up_time").html("Працюємо вже: "+myVars.uptime+"<br>Спроба №"+myVars.try);




	

//console.log(myVars);
});
}
 </script>

<script type="text/javascript">
setInterval("get_data()", 10000); 
 </script>

 
<script type="text/javascript">
function post_slider(){
	$.post( "index.cgi", { slider1: $( "#slider1" ).prop( "checked"), slider2: $( "#slider2" ).prop( "checked") } );
//slider1(this.id)	
//slider2(this.id)
get_data();
	
};
 </script>

 
 
 <script type="text/javascript">
 function load_script() {
	get_data();
 
 };
 
 </script>

 <script type="text/javascript">
 function draw_color_circle(){
	      var canvas = document.getElementById('myCanvas');
		  var context = canvas.getContext('2d');
		  var centerX = canvas.width / 2;
		  var centerY = canvas.height / 2;
		  var radius = 10;
		  context.beginPath();
		  context.arc(centerX, centerY, radius, 0, 2 * Math.PI, false); 
		  context.fillStyle = $("#myCanvas").attr("data-color");
		  context.fill();
		  context.lineWidth = 2;
		  context.strokeStyle = '#999999';
		  context.stroke();
 };
</script>


<body onload="load_script()">
<div id="main">
<table width="auto" align="center">
<tr>
<td align="right" colspan="2">
<a href="/options/options.tpl"><button name="options" class="options_btn">Налаштування</button></a>
</td>
</tr>

<tr>
<td colspan="2" align="center">
	
<h1><canvas id="myCanvas" width="23" height="23" onload="draw_color_circle()"></canvas>

ESP_IoT_WC_FAN_0201<br> 
</h1>
	
</td>
</tr>
<tr>
<img src="fan-animated.gif" >
<img src="fan-static.gif" >



</tr>
<tr>
<img src="movement.png" >




</tr>
<tr>
<td align="center" colspan="2"><div id="current_time"></div></td>
</tr>

<tr>
<td align="center" colspan="2"><div id="working_time"></div></td>
</tr>

<tr>
<td  align="center" colspan="2"><div id="temperature"></div></td>
</tr>

<tr>
<td  align="center" colspan="2"><div id="BME"></div></td>
</tr>

<tr>
<td align="right">
<div class="onoffswitch1">
<input type="checkbox" class="onoffswitch1-checkbox" id="slider1" name="slider1" onchange="post_slider()" > 
	<label class="onoffswitch1-label" for="slider1"> 
        <span class="onoffswitch1-inner"></span>
        <span class="onoffswitch1-switch"></span>
</label>
</div>
</td>

<td align="left"><font size="5" class="switch_text"><div id="checker_1_text"></div></font></td>
</tr>


<tr>
<td align="right">
<div class="onoffswitch2">
<input type="checkbox" class="onoffswitch1-checkbox" id="slider2" name="slider2" onchange="post_slider()" > 
	<label class="onoffswitch2-label" for="slider2"> 
        <span class="onoffswitch2-inner"></span>
        <span class="onoffswitch2-switch"></span>
</label>
</div>
</td>

<td align="left"><font size="5" class="switch_text"><div id="checker_2_text"></div></font></td>
</tr>



<tr><td align="center" colspan="2" class="main"><div id="up_time"></div></td></tr>

<tr>
<td>
<form method="post" action="index.cgi" id="checked1" name="checked1">
<input type="hidden" id="slider1" name="slider1" value="1">
</form>

<form method="post" action="index.cgi" id="unchecked1" name="unchecked1">
<input type="hidden" id="slider1" name="slider1" value="0">
</form>
</td>
</tr>

<tr>
<td>
<form method="post" action="index.cgi" id="checked2" name="checked2">
<input type="hidden" id="slider2" name="slider2" value="1">
</form>

<form method="post" action="index.cgi" id="unchecked2" name="unchecked2">
<input type="hidden" id="slider2" name="slider2" value="0">
</form>
</td>
</tr>
</table>
</div>

<!--
<div id="refresh">
	<div id="sensor">
		

	</div>
</div>
<button name="btn_time" type="submit" id="btn_time" class="options_btn" onclick="get_sensor_data()">Застосувати</button> -->
</body>
</html>
<!-- Some comment to save our world