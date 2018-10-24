<!DOCTYPE html>
<html><head><title>Test</title>
<link rel="stylesheet" type="text/css" href="style.css">
<!--<meta http-equiv="refresh" content="10">-->
<meta charset=UTF-8>
</head>
<body> 
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
	<h1>Час увімкнення</h1>
		<form method="post" action="set_on_time.cgi">
		<table>
		<tr>
			
			<td>
				<select name="on_time">
				<option	%on_t_0000% value="0000">00:00</option>
				<option	%on_t_0015% value="0015">00:15</option>
				<option	%on_t_0030% value="0030">00:30</option>
				<option	%on_t_0045% value="0045">00:45</option>
				<option	%on_t_0060% value="0060">01:00</option>
				<option	%on_t_0075% value="0075">01:15</option>
				<option	%on_t_0090% value="0090">01:30</option>
				<option	%on_t_0105% value="0105">01:45</option>
				<option	%on_t_0120% value="0120">02:00</option>
				<option	%on_t_0135% value="0135">02:15</option>
				<option	%on_t_0150% value="0150">02:30</option>
				<option	%on_t_0165% value="0165">02:45</option>
				<option	%on_t_0180% value="0180">03:00</option>
				<option	%on_t_0195% value="0195">03:15</option>
				<option	%on_t_0210% value="0210">03:30</option>
				<option	%on_t_0225% value="0225">03:45</option>
				<option	%on_t_0240% value="0240">04:00</option>
				<option	%on_t_0255% value="0255">04:15</option>
				<option	%on_t_0270% value="0270">04:30</option>
				<option	%on_t_0285% value="0285">04:45</option>
				<option	%on_t_0300% value="0300">05:00</option>
				<option	%on_t_0315% value="0315">05:15</option>
				<option	%on_t_0330% value="0330">05:30</option>
				<option	%on_t_0345% value="0345">05:45</option>
				<option	%on_t_0360% value="0360">06:00</option>
				<option	%on_t_0375% value="0375">06:15</option>
				<option	%on_t_0390% value="0390">06:30</option>
				<option	%on_t_0405% value="0405">06:45</option>
				<option	%on_t_0420% value="0420">07:00</option>
				<option	%on_t_0435% value="0435">07:15</option>
				<option	%on_t_0450% value="0450">07:30</option>
				<option	%on_t_0465% value="0465">07:45</option>
				<option	%on_t_0480% value="0480">08:00</option>
				<option	%on_t_0495% value="0495">08:15</option>
				<option	%on_t_0510% value="0510">08:30</option>
				<option	%on_t_0525% value="0525">08:45</option>
				<option	%on_t_0540% value="0540">09:00</option>
				<option	%on_t_0555% value="0555">09:15</option>
				<option	%on_t_0570% value="0570">09:30</option>
				<option	%on_t_0585% value="0585">09:45</option>
				<option	%on_t_0600% value="0600">10:00</option>
				<option	%on_t_0615% value="0615">10:15</option>
				<option	%on_t_0630% value="0630">10:30</option>
				<option	%on_t_0645% value="0645">10:45</option>
				<option	%on_t_0660% value="0660">11:00</option>
				<option	%on_t_0675% value="0675">11:15</option>
				<option	%on_t_0690% value="0690">11:30</option>
				<option	%on_t_0705% value="0705">11:45</option>
				<option	%on_t_0720% value="0720">12:00</option>
				<option	%on_t_0735% value="0735">12:15</option>
				<option	%on_t_0750% value="0750">12:30</option>
				<option	%on_t_0765% value="0765">12:45</option>
				<option	%on_t_0780% value="0780">13:00</option>
				<option	%on_t_0795% value="0795">13:15</option>
				<option	%on_t_0810% value="0810">13:30</option>
				<option	%on_t_0825% value="0825">13:45</option>
				<option	%on_t_0840% value="0840">14:00</option>
				<option	%on_t_0855% value="0855">14:15</option>
				<option	%on_t_0870% value="0870">14:30</option>
				<option	%on_t_0885% value="0885">14:45</option>
				<option	%on_t_0900% value="0900">15:00</option>
				<option	%on_t_0915% value="0915">15:15</option>
				<option	%on_t_0930% value="0930">15:30</option>
				<option	%on_t_0945% value="0945">15:45</option>
				<option	%on_t_0960% value="0960">16:00</option>
				<option	%on_t_0975% value="0975">16:15</option>
				<option	%on_t_0990% value="0990">16:30</option>
				<option	%on_t_1005% value="1005">16:45</option>
				<option	%on_t_1020% value="1020">17:00</option>
				<option	%on_t_1035% value="1035">17:15</option>
				<option	%on_t_1050% value="1050">17:30</option>
				<option	%on_t_1065% value="1065">17:45</option>
				<option	%on_t_1080% value="1080">18:00</option>
				<option	%on_t_1095% value="1095">18:15</option>
				<option	%on_t_1110% value="1110">18:30</option>
				<option	%on_t_1125% value="1125">18:45</option>
				<option	%on_t_1140% value="1140">19:00</option>
				<option	%on_t_1155% value="1155">19:15</option>
				<option	%on_t_1170% value="1170">19:30</option>
				<option	%on_t_1185% value="1185">19:45</option>
				<option	%on_t_1200% value="1200">20:00</option>
				<option	%on_t_1215% value="1215">20:15</option>
				<option	%on_t_1230% value="1230">20:30</option>
				<option	%on_t_1245% value="1245">20:45</option>
				<option	%on_t_1260% value="1260">21:00</option>
				<option	%on_t_1275% value="1275">21:15</option>
				<option	%on_t_1290% value="1290">21:30</option>
				<option	%on_t_1305% value="1305">21:45</option>
				<option	%on_t_1320% value="1320">22:00</option>
				<option	%on_t_1335% value="1335">22:15</option>
				<option	%on_t_1350% value="1350">22:30</option>
				<option	%on_t_1365% value="1365">22:45</option>
				<option	%on_t_1380% value="1380">23:00</option>
				<option	%on_t_1395% value="1395">23:15</option>
				<option	%on_t_1410% value="1410">23:30</option>
				<option	%on_t_1425% value="1425">23:45</option>
				</select>
			</td>
			<td>
				<button name="btn_time" class="options_btn">Застосувати</button>
			</td>
						
		</tr>

		</table>
		</form>
		<p>%showResult%</p>
	</div> 
</body>
</html>
