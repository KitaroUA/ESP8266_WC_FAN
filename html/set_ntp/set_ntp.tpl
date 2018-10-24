<!DOCTYPE html>
<html>
	<head>
		<meta name="generator"     content="HTML Tidy for HTML5 (experimental) for Windows https://github.com/w3c/tidy-html5/tree/c63cc39" />
		<title>Test</title>
		<link rel="stylesheet" type="text/css" href="style.css" />
		<meta charset="UTF-8" />
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

	
<script src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
<script>    window.jQuery || document.write('<script src="../js/jquery-3.2.1.min.js"><\/script>')</script>

	
<script type="text/javascript">
function get_pc_time(){
	var currentdate = new Date(); 
// console.log(currentdate.toISOString());
// console.log(currentdate.getUTCFullYear());
// console.log(currentdate.getUTCMonth());
// console.log(currentdate.getUTCDate());

// console.log(currentdate.getUTCHours());
// console.log(currentdate.getUTCMinutes());
// console.log(currentdate.getUTCSeconds());
 
// console.log(0 - currentdate.getTimezoneOffset());
 
$.post( "PC_Time.cgi",{data:currentdate.getUTCFullYear()+","+currentdate.getUTCMonth()+","+currentdate.getUTCDate()+","+currentdate.getUTCHours()+","+currentdate.getUTCMinutes()+","+currentdate.getUTCSeconds()+","+currentdate.getUTCDay()});

 
};
 
 </script>

	
			<form method="post" action="set_ntp.cgi">
				<table>
					<tr>
						<th align="left">Host name:             <input name="hn" value="%v_hostname%" size="12" />
						</th>
						<th align="right">IP NTP:             <input name="ntpip" value="%v_ntp%" size="14" />
						</th>
					</tr>
					<tr>
						<th align="left">GMT zone:             <input value="%v_timezone%" name="timezone" size="3" />
						</th>
						<th align="left">Перехід на літній час:             <input type="radio" name="DST" value="1" %DST_1%/> Так             <input type="radio" name="DST" value="0" %DST_2%/> Ні</th>
						<th align="right">Формат NTP:             <input type="radio" name="ntp_type" value="0" %ntp_type_1%/> IP             <input type="radio" name="ntp_type"  value="1" %ntp_type_2%/> DNS</th>
					</tr>
					<tr>
						<th align="left">
							<button name="btn_time" class="options_btn">Застосувати</button>
						</th>
						<th align="right">%v_ntpresult%</th>
					</tr>
				</table> 
			</form>
			<form method="post" action="apply_ntp.cgi">
				<table>
					<tr>
						<th align="left">
							<button name="btn_time" class="options_btn">Оновити час</button>
						</th>

					</tr>
					<tr>
						
					</tr>
					
				</table>
			</form>
							<p align="left" style="font-size:10px">Увага, оскільки бібліотека time.h в ESP8266 реалізована дуже кострубато, перевірки часу на 
							              цілісність не відбувається, перевод на літній час - тільки "вручну", або через пересинхронізацію
										  NTP, або зміною часового поясу. При цьому треба мати на увазі, якщо часова зона + перехід
										  на літній час змінюють дату - будуть глюки, оновлювати час тільки тоді, коли після застосування
										  всіх модифікаторів він залишиться в межах поточної доби.
										  </p>
										<button name="btn_time" class="options_btn" onclick="get_pc_time()">Застосувати час з комп'ютера</button>


		</div>
	</body>
</html> 