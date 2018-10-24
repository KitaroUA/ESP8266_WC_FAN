<html>
  <head>
    <meta charset="UTF-8" />
	
	
	
	
	<!--	<script src="../js/jquery-3.2.1.min.js"></script> -->
	<script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>
  <!--	<script src="https://code.jquery.com/ui/1.12.1/jquery-ui.js"></script> -->

   <script src="../js/jquery.radios-to-slider.min.js"></script>
   <link rel="stylesheet" href="../css/radios-to-slider.min.css">

  
  
  
  <link rel="stylesheet" href="../css/jquery.range.css">
  <script src="../js/jquery.range.js"></script>

	

	
	
	
	<script type="text/javascript" src="temperature_selector_options_customize.js"></script>	
  	<script type="text/javascript" src="temperature_selector_1.js"></script>
	
 <script type="text/javascript">
    $(document).ready(function(){

       $("#radios").radiosToSlider({
					  size: 'medium',
					  animation: true,
					  fitContainer: true,
					  isDisable: false,
					  onSelect: null
   });
	$("#radios").bind("radiochange", function () { 
//		console.log( $("#radios").attr("data-value"));
		var data=$("#main").attr('data-exchange');
		

		var myVars;
		var line=data.split(',');
		myVars={
        'temparture_sensor':line[0],
        'control_channel':line[1],
        'on_temperature':line[2],
        'off_temperature':line[3],
        'lower_dimmer_value':line[4],
        'upper_dimmer_value':line[5]
        }

		if ($("#radios").attr("data-value") == 0) // Turn off
		{
		myVars.control_channel = 255;
		}

		if ($("#radios").attr("data-value") == 1) // Relay
		{
		myVars.control_channel = 0;
		}

		if ($("#radios").attr("data-value") == 2) // PWM
		{
		myVars.control_channel = 100;
		}
		temperature_selector_options_customize(myVars.temparture_sensor+','+myVars.control_channel+','+myVars.on_temperature+','+myVars.off_temperature+','+myVars.lower_dimmer_value+','+myVars.upper_dimmer_value);	

		});	
   
 $('.temperature_range').jRange({
    from: 19,
    to: 29,
    step: 0.1,
	scale: [19,20,21,22,23,24,25,26,27,28,29],
    width: 600,
    showLabels: true,
    isRange : true
});
 
 
$('.PWM_LOW').jRange({
    from: 0,
    to: 255,
    step: 1,
    scale: [0,64,128,196,255],
    width: 300,
    showLabels: true,
    snap: true
}); 

$('.PWM_HIGH').jRange({
    from: 0,
    to: 255,
    step: 1,
    scale: [0,64,128,196,255],
    width: 300,
    showLabels: true,
    snap: true
});  




option_1('Temperature_options_1',$('#main').attr('data-opt_sel'),-1);
	
// Hide all components
	$("#Temperature_sensor_selector").hide();
	$("#Output_channel_selector").hide();
	$("#radios").hide();
	$("#temperature_range_cell").hide();
	$("#PWM_LOW_cell").hide();
	$("#PWM_HIGH_cell").hide();
});
</script>

 <script type="text/javascript">
function options_selector_event(){	
	$("#radios").hide();
	
	
val = $('#Temperature_options_1').val();
//console.log(val);
$('#Temperature_options_1_cgii').val(val);
//$.post( "Temperature_options_1.cgi", {'Temperature_options_1_cgii':$('#Temperature_options_1').val()});
//document.getElementById("Temperature_options_1_cgi").submit();	

$.get('temperature_selector.var', { val }, function (data) {
	
temperature_selector_options_customize(data);	
	
});

}



 </script>	 
 
 <script type="text/javascript">
 function myFn(){
 Невірно!!!! треба не вичитувати з заголовку, а перекомпілювати
 $.post( "Temperature_options_1.cgi",{data:$('#Temperature_options_1').val() +","+$("#main").attr('data-exchange')});
};
 
 </script>

   <link rel="stylesheet" type="text/css" href="style.css" />
   
   <title></title>
  </head>
  <body >
    <div id="main"  data-opt_sel=%s_opt% data-opt_rel=%r_opt% data-opt_pwm=%p_opt% data-opt_temper=%t_opt% data-exchange="">
        <table style="text-align: left; width: 800px; height: 86px;" >
          <tbody>
           <tr>
              <td >
			  <select name="Temperature_options_1" id="Temperature_options_1"  onchange='options_selector_event()'></select>
			  </td>
              <td ></td>
              <td ></td>
            </tr>
            <tr>
              <td >
			  	<div id="radios" name="radios" >
				<input id="option1" name="options" type="radio" value="1" >
				<label for="option1"> Реле </label>
				<input id="option2" name="options" type="radio" value="0" >
				<label for="option2"> Вимк. </label>
				<input id="option3" name="options" type="radio" value="2" >
				<label for="option3"> ШІМ </label>
				</div> 

 
			  </td>
              <td >
				  <p id="i1" style="display:none"> 1 </p> 
				  <p id="i2" style="display:none"> 2 </p> 
			 </td>
              <td ></td>
            </tr>
 
 
 
 
            <tr>
				<td >
			  <select name="Temperature_sensor_selector" id="Temperature_sensor_selector"  ></select>
			  </td>
              <td >
			<select name="Output_channel_selector" id="Output_channel_selector"  ></select>
			</td>
              <td ></td>
            </tr>

 
 
 
			<tr>
              <td >			  

			  
			  
			  
			
				</td>
              <td></td>
              <td >
               <button name="btn_time" type="submit" id="btn_time" class="options_btn" onclick="myFn()">Застосувати</button>
				  </td>
            </tr>
			<tr>
			
			<td height = "100" colspan = 3  id="temperature_range_cell">
				<input id="temperature_range" type="hidden" class="temperature_range" />	
				
				</td>
			<td>
				
				</td>
			<td></td>
			
			</tr>
 
			<tr>
			
			<td id="PWM_LOW_cell">
				<input id="PWM_LOW" type="hidden" class="PWM_LOW"  />

			</td>
			<td>
			</td>
			<td id="PWM_HIGH_cell">				
			<input id="PWM_HIGH" type="hidden" class="PWM_HIGH"  />

			</td>
			
			</tr>
 
 
 
 
 
		</tbody>
        </table>

    </div>



  </body>
</html>

<!-- Some comment to save our world -->
<!-- Some comment to save our world -->
<!-- Some comment to save our world -->
<!-- Some comment to save our world -->
<!-- Some comment to save our world -->
<!-- Some comment to save our world 