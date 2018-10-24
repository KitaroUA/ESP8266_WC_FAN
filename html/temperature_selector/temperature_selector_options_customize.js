


function temperature_selector_options_customize(data) {
/*	
    var perLine=data.split(';');
    var myVars=[];
    for(i=0;i<perLine.length;i++)
    {
    var line=perLine[i].split(' ');
    myVars[i]={
        'time':line[0],
        'event':line[1],
        'color':line[2]
        }
    }
    console.log(myVars);
    console.log(myVars[0].time);
    console.log(myVars[0].event);
    console.log(myVars[0].color);	
});*/


//    var perLine=data.split(';');
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
		$("#main").attr('data-exchange',myVars.temparture_sensor+','+myVars.control_channel+','+myVars.on_temperature+','+myVars.off_temperature+','+myVars.lower_dimmer_value+','+myVars.upper_dimmer_value);
		console.log(myVars);
	console.log(parseInt(myVars.control_channel));
	
	
$('#temperature_range').jRange('setValue',(parseInt(myVars.on_temperature)/10000)+','+ (parseInt(myVars.off_temperature)/10000));
		
	option_1('Temperature_sensor_selector',$('#main').attr('data-opt_temper'),myVars.temparture_sensor);

  $("#Temperature_sensor_selector").show();
$("#Output_channel_selector").show(); 

	
	$("#radios").attr("data-value", $('#Temperature_options_1').val());
   
	if (parseInt(myVars.control_channel) == 255)
	{
	$('input:radio[name="options"][value=0]').prop('checked', true); 
	       $("#radios").radiosToSlider({
					  size: 'medium',
					  animation: true,
					  fitContainer: true,
					  isDisable: false,
					  onSelect: null
   });
   	$("#Temperature_sensor_selector").hide();
	$("#Output_channel_selector").hide();
	$('#temperature_range_cell').hide();
	$('#PWM_LOW_cell').hide();
	$('#PWM_HIGH_cell').hide();


	$("#radios").show();
	}
	else
	{
		if (parseInt(myVars.control_channel) >= 100)
		{
		$('input:radio[name="options"][value=2]').prop('checked', true); 
	       $("#radios").radiosToSlider({
					  size: 'medium',
					  animation: true,
					  fitContainer: true,
					  isDisable: false,
					  onSelect: null
   });
		option_1('Output_channel_selector',$('#main').attr('data-opt_pwm'),parseInt(myVars.control_channel)-100);
		$('#PWM_LOW').jRange('setValue',(myVars.lower_dimmer_value));
		$('#PWM_HIGH').jRange('setValue',(myVars.upper_dimmer_value));
		$('#temperature_range_cell').show();
		$('#PWM_LOW_cell').show();
		$('#PWM_HIGH_cell').show();

		$("#radios").show();
		}
		else
		{
			if (parseInt(myVars.control_channel) < 100)
			{
			$('input:radio[name="options"][value=1]').prop('checked', true);
	       $("#radios").radiosToSlider({
					  size: 'medium',
					  animation: true,
					  fitContainer: true,
					  isDisable: false,
					  onSelect: null
   });
			option_1('Output_channel_selector',$('#main').attr('data-opt_rel'),myVars.control_channel);
			$("#radios").show();
			$('#temperature_range_cell').show();
			$('#PWM_LOW_cell').hide();
			$('#PWM_HIGH_cell').hide();

			}
		}
	}
}
//Some comment to protect or world
//Some comment to protect or world
//Some comment to protect or world
//Some comment to protect or world
//Some comment to protect or world
//Some comment to protect or world