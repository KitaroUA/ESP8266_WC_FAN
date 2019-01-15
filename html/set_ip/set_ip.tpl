<!DOCTYPE html>
<html>
	<head>
		<meta name="generator"     content="HTML Tidy for HTML5 (experimental) for Windows https://github.com/w3c/tidy-html5/tree/c63cc39" />
		<title>Test</title>
		<link rel="stylesheet" type="text/css" href="style.css" />
		<meta charset="UTF-8" />
		
	<!--		<script src="https://code.jquery.com/jquery-1.12.4.min.js"></script> -->
	<!-- <script src="https://code.jquery.com/jquery-1.12.4.min.js"></script> 
			<script src="../js/jquery.ipinput.js"></script>-->
<script>
//    if (typeof jQuery == 'undefined') {
//        document.write(unescape("%3Cscript src='../js/jquery.min.js' type='text/javascript'%3E%3C/script%3E"));
//    }
</script>


	<!--<script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>-->
<script src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
			
<script>    window.jQuery || document.write('<script src="../js/jquery-3.2.1.min.js"><\/script>')</script>

<script src="../js/jquery.ipinput.js"></script>

<script>
function myFn_ip(){	
$.post( "set_ip_1.cgi", {'sta_ip_addr':$('input[name=sta_ip_addr_i]').val(),
						 'sta_ip_mask':$('input[name=sta_ip_mask_i]').val(),
						 'sta_ip_gw'  :$('input[name=sta_ip_gw_i]'  ).val(),
						 
						 'sap_ip_addr':$('input[name=sap_ip_addr_i]').val(),
						 'sap_ip_mask':$('input[name=sap_ip_mask_i]').val(),
						 'sap_ip_gw'  :$('input[name=sap_ip_gw_i]'  ).val(),
						 'DHCP'       :$('#DHCPcheckBox').is(":checked")});
}
</script>



	</head>
	<body>
		<div id="main" >
	<table width="800px">
		<tr>
			<td align="right">
			   <p>
				<a href ="/index.tpl"><button name="go_back" class="options_btn">Повернутися</button> </a>
			  </p>
			</td>
		</tr>
	</table>
				<table>
				<tr>
				<input id="DHCPcheckBox" name="DHCPcheckBox" type="checkbox" %dhcp_checked%> <label>  <b>DHCP</b></label>
				</tr>
					<tr>
						<th align="left">Station ip: 
						</th>
						<th>
						
						<div id="sta_ip_addr" class="ipinput" data-name='sta_ip_addr_i' data-ip=%sta_ip_addr%>
					
						<script>
						$('#sta_ip_addr').ipinput();
						</script>
						</div>
						</th>

						
					</tr>
					<tr>
						<th align="left">Station ip mask:            
						</th>
						<th>
						
						<div id="sta_ip_mask" class="ipinput" data-name='sta_ip_mask_i' data-ip=%sta_ip_mask%>
					
						<script>
						$('#sta_ip_mask').ipinput();
						</script>
						</div>
						</th>

					</tr>
					<tr>
						<th align="left">Station gateway:             
						</th>
						<th>
						
						<div id="sta_ip_gw" class="ipinput" data-name='sta_ip_gw_i' data-ip=%sta_ip_gw%>
					
						<script>
						$('#sta_ip_gw').ipinput();
						</script>
						</div>
						</th>
						
					</tr>
					
					
					<tr>
						<th align="left">Soft AP ip: 
						</th>
						<th>
						
						<div id="sap_ip_addr" class="ipinput" data-name='sap_ip_addr_i' data-ip=%sap_ip_addr%>
					
						<script>
						$('#sap_ip_addr').ipinput();
						</script>
						</div>
						</th>

						
					</tr>
					<tr>
						<th align="left">Soft AP mask:            
						</th>
						<th>
						
						<div id="sap_ip_mask" class="ipinput" data-name='sap_ip_mask_i' data-ip=%sap_ip_mask%>
					
						<script>
						$('#sap_ip_mask').ipinput();
						</script>
						</div>
						</th>

					</tr>
					<tr>
						<th align="left">Soft AP gateway:             
						</th>
						<th>
						
						<div id="sap_ip_gw" class="ipinput" data-name='sap_ip_gw_i' data-ip=%sap_ip_gw%>
					
						<script>
						$('#sap_ip_gw').ipinput();
						</script>
						</div>
						</th>
						
					</tr>

				</table> 
			<!--			<form method="post" action="set_ip_1.cgi">-->
				<table width="800px">
					<tr>
						<th align="left">
							<button name="set_ip" class="options_btn" onclick="myFn_ip()">Записати данні в пам'ять</button>
						</th>
						<th align="right">
						<form method="post" action="set_ip_2.cgi">
							<button name="set_ip" class="options_btn" >Застосувати</button>
						</form>
						</th>
					</tr>
				</table>
<!--			</form> -->
		</div>

		
	</body>
	

</html> 
<!-----