const char Menu[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <style>
      body {background-color: rgb(238, 118, 232);}
    </style>
  </head>

<body>
  <center>
    <h1>iTrolley Menu</h1>
    <p>Please select what you would like to do</p>
    <a href = "/input"><button>Configure mDNS</button></a>
    <a href = "/sensor"></a><button>Monitor Sensors</button></a>
    
  </center>
</body>
</html>
)=====";

const char mem_clear[] PROGMEM = R"=====(
<!DOCTYPE html>
	<html>
		<head>
			<style>
			body { background-color: rgb(238, 118, 232);}
			</style>
				<script>
					setTimeout(function(){
						window.open("http://itrolley.local/");
					},2000);
				</script>
		</head>
			<center>
				<body>
					<h1>EEPROM memory has been cleared</h1><br> 
					<p>Please give the trolley 2-3 mins to reboot</p>
				</body>
			</center>
	</html>
		)====="; 


	const char inputnum[] PROGMEM = R"=====(
	<!DOCTYPE html>
  <html>
    <head>
      <style>
        body {background-color: rgb(238, 118, 232);}
      </style>
        <script type = "text/javascript"> 
          function al(){
          let v1 = document.getElementById("hostname").value;
        if (v1 == "") {}
        else {
            alert("The mDNS name has been changed to: \n" + v1 + "\nPlease give 2-3 mins to reboot the system ");
            window.open("http://" + v1 + ".local/")
        }
        }
        </script>
  </head>
    <body>
      <center>
        <h2>Trolley Number Input</h2>
          <p>Input the name of the iTrolley <br>This will change the mDNS name of the trolley</p>
            <form>
              Enter The Trolley Name:
              <input type="text" name= "trolleynumber" size = "12" placeholder = "itrolley" maxlength = "12" id = "hostname">
              <br><br>
              <input type = "submit" value = "Submit" onclick = "al()">
            </form>
          <br><a href="/memclear"><button>Memory Clear</button></a>
        <a href="/"><button>Back</button></a>
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
