<!DOCTYPE html>
<html>
	<head>
		<meta name="generator"     content="HTML Tidy for HTML5 (experimental) for Windows https://github.com/w3c/tidy-html5/tree/c63cc39" />
		<title>Test</title>
		<script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>
		<link rel="stylesheet" type="text/css" href="style.css" />
		<meta charset="UTF-8" />
	</head>
	<script type="text/javascript">
 function myMQTTStatus(){
 var val;
// console.log($("#mqtt_checkbox").is(":checked"));
 if ($("#mqtt_checkbox").is(":checked"))
 {
 //console.log("checked");
 $("#main").attr('data-mqtt_status',"1");
 $("#mqtt_status").val("1");
 }
 else
 {
 $("#main").attr('data-mqtt_status',"0");
// console.log("UNchecked");
 $("#mqtt_status").val("0");

 }
 
 


};	
	
	
	
	
	
	
	</script>
	
	 <script type="text/javascript">
    $(document).ready(function(){
	var val;
	val = $("#main").attr('data-mqtt_status');
//	console.log(val);
	if (val =="0")
	{
	$("#mqtt_checkbox").prop('checked', false);
	$("#mqtt_status").val("0");
	}
	else
	{
	$("#mqtt_checkbox").prop('checked', true);
	$("#mqtt_status").val("1");
	}
	});
	</script>
	<body>
		<div id="main" data-mqtt_status=%mqtt_status%>
	<table width="800px">
		<tr>
			<td align="right">
			   <p>
				<a href ="/index.tpl"><button name="go_back" class="options_btn">Повернутися</button> </a>
			  </p>
			</td>
		</tr>
	</table>
			<form method="post" action="set_mqtt.cgi">
				<table>
					<tr>
						<th align="left">IP MQTT:             <input name="mqttip" value="%v_mqtt%" size="14" />
						</th>
					</tr>
					<tr>
						<th align="right">Активувати таймерну процедуру переачі даних MQTT: <input type="checkbox" name="mqtt_checkbox" id="mqtt_checkbox" value="0" onchange="myMQTTStatus()"/> 
						<input type="hidden" name="mqtt_status" id="mqtt_status" value="0"/>
          </th>
					</tr>
					<tr>
						<th align="left">
							<button name="btn_time" class="options_btn">Застосувати</button>
						</th>
					</tr>
				</table> 
		</div>
	</body>
</html> 
<!----