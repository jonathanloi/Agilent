const char mem_clear[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<style>
.h1 {
  background-color: rgb(5, 187, 187);
  font-family: 'Courier New', Courier, monospace;
  color: black;
  padding: 30px;
  text-align: center;
}
</style>
  <script>
  window.open("http://itrolley.local/");
  </script>
</head>
<div class="h1">
  <body>
  <h1>EEPROM memory has been cleared</h1><br> 
  <p>Please give the trolley 2-3 mins to reboot</p>
  </body>
</div>
</html>
)=====";


const char configureName[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<style>

h1 {
  background-color: rgb(5, 187, 187);
  padding: 25px;
}
.bod{
  font-family: 'Courier New', Courier, monospace;
  color: black; 
  text-align: center;
}
.text {
  font-family: 'Courier New', Courier, monospace;
  background-color: #555;
  color: white;
  float: left;
  border: none;
  outline: none;
  cursor: pointer;
  padding: 10px;
  font-size: 20px;
}
.text:hover {
  background-color: #777;
}
.textt{
  font-family: 'Courier New', Courier, monospace;
  background-color: #555;
  color: white;
  border: none;
  outline: none;
  cursor: pointer;
  padding: 10px;
  font-size: 20px;
  align-content: center;
}
.textt:hover{
  background-color: #777;
}

</style>
<script type = "text/javascript"> 
  function al(){
    let v1 = document.getElementById("hostname").value;
    if (v1 != "" && v1 != "itrolley") {
      alert("The mDNS name has been changed to: \n" + v1 + "\nPlease give 2-3 mins to reboot the system ");
       window.open("http://" + v1 + ".local/")
     }
  }
</script>
</head>
<body>
  <a href="/"><h3 class="text">Back</h3></a>
  <div class="bod">
  <h1 class="h1">Configure iTrolley</h1>
  <p>Input the name of the iTrolley <br>This will change the mDNS name of the trolley</p>
  <form>
    Enter The Trolley Name:
    <input type="text" name= "trolleynumber" size = "12" placeholder = "itrolley" maxlength = "12" id = "hostname">
    <br><br>
    <input type = "submit" value = "Submit" onclick = "al()" class="textt">
  </form>
  <br><a href="/memclear"><button class="textt">Memory Clear</button></a>
  </div>  
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

const char Menu[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8" />
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<title>Page Title</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" type="text/css" media="screen" href="main.css" />
<script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.2/Chart.bundle.js"></script>
<style>

.table{
  border: 2px solid black;
  border-collapse: collapse;
  float:left;  
  margin-left: 10px;
  margin-top: 2px;
    }
.td {
  padding: 5px;
  width: 250px;
  font-family: 'Courier New', Courier, monospace;
    }
.h1 {
  background-color: rgb(5, 187, 187);
  font-family: 'Courier New', Courier, monospace;
  color: black;
  padding: 25px;
  text-align: center;
    }
.val {
  padding: 5px;
  font-family: 'Courier New', Courier, monospace;
  width: 90px;
  text-align: center;

    }
.text {
  font-family: 'Courier New', Courier, monospace;
  background-color: #555;
  color: white;
  border: none;
  outline: none;
  cursor: pointer;
  padding: 5px;
}
.text:hover {
  background-color: #777;
}

</style>
    
</head>
<body>
<h1 class="h1"><a href="/configure" class="text">iTrolley</a> Overview</h1>
<div class="table">
<table>
  <tr>
    <td class="td">Assigned Serial No. : </td>
    <td class="val">1 </td>
  </tr>
  <tr>
    <td class="td">Instrument's Presence  :</td>
    <td class="val">3 </td>
  </tr>
  <tr>
    <td class="td">AC Current  :</td>
    <td class="val"> 5</td>
  </tr>
  <tr>
    <td class="td">Location  :</td>
    <td class="val">2 </td>
  </tr>
  <tr>
    <td class="td">Bay  :</td>
    <td class="val">3 </td>
  </tr>
</table>
</div>
<div class="table">
<table>
  <tr>
    <td class="td">Assigned Serial No. : </td>
    <td class="val">1 </td>
  </tr>
  <tr>
    <td class="td">Instrument's Presence  :</td>
    <td class="val">3 </td>
  </tr>
  <tr>
    <td class="td">AC Current  :</td>
    <td class="val"> 5</td>
  </tr>
  <tr>
    <td class="td">Location  :</td>
    <td class="val">2 </td>
  </tr>
  <tr>
    <td class="td">Bay  :</td>
    <td class="val">3 </td>
  </tr>
</table>
</div>
  <canvas id="myChart"></canvas>
    <script>
        var ctx = document.getElementById('myChart').getContext("2d");
        let chart = new Chart(ctx, {
        // The type of chart we want to create
        type: 'line',
        // The data for our dataset
        data: {
        labels: [1750,1800,1850,1900,1950,1999,2050],
        datasets: [{ 
        data: [0, 0, 0, 0, 0, 0, 0],
        label: "Temperature",
        borderColor: "#3e95cd",
        backgroundColor: "#3e95cd",
        fill: false
      }, { 
        data: [1, 1, 1, 1, 1, 1, 1],
        label: "Current",
        borderColor: "#8e5ea2",
        backgroundColor: "#8e5ea2",
        fill: false
      }, { 
        data: [6, 3, 2, 2, 7, 26, 82],
        label: "North America",
        borderColor: "#c45850",
        backgroundColor: "#c45850",
        fill: false
      }
    ]
  },
    
    // Configuration options go here
    options: {}
    });     
      
    </script>     
</body>
</html> 


)=====";
