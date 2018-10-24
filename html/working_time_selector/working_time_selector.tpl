<html>
		<!--http://www.jqueryscript.net/time-clock/Android-Clock-Time-Picker-Plugin-jQuery.html --->

  <head>
    <meta name="generator"
    content="HTML Tidy for HTML5 (experimental) for Windows https://github.com/w3c/tidy-html5/tree/c63cc39" />
    <meta charset="UTF-8" />
  <!--  	<script type="text/javascript" src="working_time_selector_1.js"></script> --->
	<!--	<script src="../js/jquery-3.2.1.min.js"></script> -->
	<script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>
<script src="../js/jquery-clock-timepicker.min.js"></script>
<script type="text/javascript">
    $(document).ready(function() {
	  $('.time_to_on').clockTimePicker({precision:5});
	});
	
    $(document).ready(function() {
	  $('.time_to_off').clockTimePicker({precision:5});
	});

	function myFunction_on(v) 
	{document.getElementById("sh_on").value = v};

  </script>
  <script>
  	function myFunction_off(v) 
	{document.getElementById("sh_off").value = v};
  </script>

	
	
	
   <link rel="stylesheet" type="text/css" href="style.css" />
    <title></title>
  </head>
  <body> <!-- onload='option_1(%time_to_on_select%,"time_to_on"); option_1(%time_to_off_select%,"time_to_off");'> --->
    <div id="main">
        <table style="text-align: left; width: 800px;" >
          <tbody>
 				<tr><td align="right" colspan="2">
					<a href="/index.tpl">
						<button name="go_back" class="options_btn">Повернутися</button>
					</a>
				</td></tr>
           <tr>
              <td >
				Час увімкення:
			  </td>
              <td >      
				<input class="time time_to_on" name="time_to_on" id="time_to_on" type="text" value=%time_to_on_select% onchange= "myFunction_on(this.value)"/>			  
			  <!--			   <select name="time_to_on" id="time_to_on"  ></select> --->
			 </td>
              <td ></td>
            </tr>

			
			 <tr>
              <td >
				Час вимкнення:
			  </td>
              <td >      
				<input class="time time_to_off" name="time_to_off" id="time_to_off" type="text" value=%time_to_off_select% onchange= "myFunction_off(this.value)"/>			  
			  <!--			   <select name="time_to_off" id="time_to_off"  ></select> --->
			 </td>
              <td ></td>
            </tr>

			
			
            <tr>
              <td >
				<form method="post" action="working_time_selector_1.cgi">
				<input type="hidden" id="sh_on"  name="sh_on"  value = %time_to_on_select%/>
				<input type="hidden" id="sh_off" name="sh_off" value = %time_to_off_select%/>
               <button name="btn_time" class="options_btn">Застосувати</button>
				</form>
			  </td>
              <td ></td>
              <td ></td>
            </tr>
 			<tr>
              <td >
			  </td>
              <td></td>
              <td ></td>
            </tr>
          </tbody>
        </table>
      <br />
    </div>
  </body>
</html>
<!-----