/*const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <style>
      body {background-color: plum;}
    </style>
  </head>

<body>
  <center>
    <h1>This is the Web Server for iTrolley!</h1>
    <p>This is trolley number zero</p>
      <a href = "/input"><button>Input</button></a>
  </center>
</body>
</html>
)=====";

*/
		const char mem_clear[] PROGMEM = R"=====(
		<!DOCTYPE html>
		<html>
		  <head>
			<style>
			  body { background-color: lightpink;}
			</style>
		  </head>
			<center>
				<body>
				<h1>EEPROM memory has been cleared</h1><br> 
				<p>Please give the trolley 2-3 mins to reboot</p>
				</body>
			<!--<br> <a href = '/'><button>Back</button></a>-->
			</center>
		</html>

		)====="; 


	const char inputnum[] PROGMEM = R"=====(
	<!DOCTYPE html>
	<html>
	  <head>
	  <style>
		body {background-color: LIGHTPINK;}
	  </style>
		<script> 
			function al(){
				alert("The mDNS name has been changed \n Please give the trolley 2-3 mins to reboot");
				
			}
		</script>
	  </head>
			<body>
			<center>
				<h2>Trolley Number Input</h2>
				<p>Input the name of the iTrolley <br><br>
					This will change the mDNS name of the trolley
				</p>
					<form>
						Enter The Trolley Number:
						<input type="text" name= "trolleynumber" size = "12" placeholder = "itrolley" maxlength = "12">
						<br><br>
						<input type= "submit" onclick = "al()">
					</form>
				<br><a href="/memclear"><button>Memory Clear</button></a>
			</center>
			</body>
	</html>
	)=====";


				const char Error[] PROGMEM = R"=====(
				<!DOCTYPE html>
				<html lang="en">
				<head>
					<meta charset="utf-8">
					<title>Page Not Found</title>
					<meta name="viewport" content="width=device-width, initial-scale=1">
					<style>
						* {
							line-height: 1.2;
							margin: 0;
						}
						html {
							color: #888;
							display: table;
							font-family: sans-serif;
							height: 100%;
							text-align: center;
							width: 100%;
						}
						body {
							display: table-cell;
							vertical-align: middle;
							margin: 2em auto;
						}
						h1 {
							color: #555;
							font-size: 2em;
							font-weight: 400;
						}
						p {
							margin: 0 auto;
							width: 280px;
						}
						@media only screen and (max-width: 280px) {
							body, p {
								width: 95%;
							}
							h1 {
								font-size: 1.5em;
								margin: 0 0 0.3em;
							}
						}
					</style>
				</head>
				<body>
					<h1>Page Not Found</h1>
					<p>Sorry, but the page you were trying to view does not exist.</p>
				</body>
				</html>
				)=====";
