<!DOCTYPE html>
<html>
  <head>
    <meta name="generator"
    content="HTML Tidy for HTML5 (experimental) for Windows https://github.com/w3c/tidy-html5/tree/c63cc39" />
    <title>Test</title>
    <script type="text/javascript" src="off.js"></script>
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
      <form method="post" action="set_temp_off.cgi">
        <input type="range" name="temp_off" id="temp_off" min="0" max="90" step="5" value=%v_tempOff% onchange="myFunction_off()" class="slider-width100" />
        <p>Вимкнути на час:</p>
        <p id="sh_off"></p>
         <button name="btn_time" class="options_btn">Застосувати</button>
      </form>
    </div>
  </body>
</html>
