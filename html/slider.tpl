<!DOCTYPE html>
<html>
  <head>
    <meta name="generator"
    content="HTML Tidy for HTML5 (experimental) for Windows https://github.com/w3c/tidy-html5/tree/c63cc39" />
    <title>Test</title>
	<script src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
	<script>    window.jQuery || document.write('<script src="../js/jquery-3.2.1.min.js"><\/script>')</script>
	<link rel="stylesheet" type="text/css" href="style.css">

    <script type="text/javascript"> <!---- src="on.js"> -->
	function myFunction_show() {
		var t = document.getElementById("slider_pos").value;
		document.getElementById("sh_on").innerHTML = "" + t;}
	
	
	</script>
   <script type="text/javascript"> <!---- src="on.js"> -->
	function myFunction_post() {


		myFunction_show();
		var t = document.getElementById("slider_pos").value;
		$.post( "./set_slider.cgi", {slider_pos: $("#slider_pos").val()});
		}
	
	</script>

    <meta charset="UTF-8" />
  </head>
  <body onload="myFunction_show()">
    <div id="main">
	<table width="800px">
		<tr>
			<td align="right">
			   <p>
				<a href ="/index.tpl"><button name="go_back" class="options_btn">Повернутися</button> </a>
			  </p>
			</td>
		</tr>
	</table>
      
        <input type="range" name="slider_pos" id="slider_pos" min="0" max="100" step="1" value=%v_slider_pos% onchange="myFunction_post()" class="slider-width100" />
        <p>Увімкнути на час:</p>
        <p id="sh_on"></p>
         <button name="btn_time" class="options_btn">Застосувати</button>
      
    </div>
  </body>
</html>
