<!DOCTYPE html>
<html>
	<head><title>IoT Light Switch</title>
		<meta charset=UTF-8>
		<link rel="stylesheet" type="text/css" href="style.css">
	</head>
	<body>
		<div id="main">
			
			<h1>
				Налаштування
			</h1>
			
			<table width="800px">
				<tr><td align="right" colspan="2">
					<a href="/index.tpl">
						<button name="go_back" class="options_btn">Повернутися</button>
					</a>
				</td></tr>
				
				<tr>
					<td align="right" colspan="2">
					<a href ="/wifi/wifi.tpl"> <button name="wifi" class="options_btn"> Налаштувати WiFi  </button> </a></li>
				</td></tr>
				<tr ><td>
					<li>Налаштування IP</li>         
					</td><td align="right">    
					<a href ="/set_ip/set_ip.tpl" > <button name="ip" class="options_btn"> Налаштувати IP  </button> </a></li>
				</td></tr>

				<tr><td>
					<li>Поточний час %curTime%</li>     
					</td><td align="right">    
					<a href ="/set_ntp/set_ntp.tpl"><button name="set_ntp" class="options_btn">Налаштувати NTP</button></a> 
				</td></tr>
				<tr><td>
					<li>Налаштування режиму роботи</li>         
					</td><td align="right">    
					<a href ="/working_time_selector/working_time_selector.tpl"><button name="working_time_selector" class="options_btn">Налаштувати</button> </a> 
				</td></tr>

				<tr><td>
					<li>Налаштування MQTT</li>         
					</td><td align="right">    
					<a href ="/set_mqtt/set_mqtt.tpl"><button name="set_mqtt" class="options_btn">Налаштування</button> </a> 
				</td></tr>
				
				
				
				
				
				
		</table>
		
	</div>
</body></html>
