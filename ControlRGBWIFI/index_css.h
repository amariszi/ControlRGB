const char PAGE_index_css[] PROGMEM = R"=====(
.slider{
    display: block;
    margin-top: 50px !important;
    margin-left: 5% !important;
    margin-right: 5% !important;
    width: 90% !important;
}
html, body{
	background-color:black;
}
#barra_superior{                
	position:relative;
	height:60px;
}
#logo{
	background-image: url('data:image/png;base64,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');  
	height: 100px;
	background-repeat: no-repeat;            
	width: 100px;
	background-size: cover;
	position: absolute;
	left: -20px;
	top: -20px;
}
#marca{
	color: white;
	float: right;
	font-size: 45px;
	font-family: fantasy; 
	position: absolute;
	right: 10px;
	bottom: 0px;
}

#muestra_color{
	position: absolute;
	left: 10px;
	bottom: 10px;
	right: 10px;
	height: 15%;
	border-radius: 5px 5px 5px 5px;
	text-align: center;
	font-size: 250%;
	padding-top: 1%;
	box-sizing: border-box;
	color: black;
	background-color: white;
}

#txt_correccion_rojo{
	border: 2px red solid;
	width: 70px;
}
#txt_correccion_verde{
	border: 2px green solid;
	width: 70px;
}
#txt_correccion_azul{
	border: 2px blue solid;
	width: 70px;
}
#lbl_ajustes{
	display: inline-block;
	color: yellow;
}
#red_slider::-webkit-slider-runnable-track{
	background: red;
}
#green_slider::-webkit-slider-runnable-track{
	background: green;
}
#blue_slider::-webkit-slider-runnable-track{
	background: blue;
}

#red_slider::-moz-range-track{
	background: red;
}
#green_slider::-moz-range-track{
	background: green;
}
#blue_slider::-moz-range-track{
	background: blue;
}

#panel_correciones{
	margin-top: 39px;
    margin-left: 5%;
}
input[type=range] {
  -webkit-appearance: none;
  margin: 18px 0;
}
input[type=range]:focus {
  outline: none;
}
input[type=range]::-webkit-slider-runnable-track {
  height: 8.4px;
  cursor: pointer;
  animate: 0.2s;
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
  border-radius: 1.3px;
  border: 0.2px solid #010101;
}
input[type=range]::-webkit-slider-thumb {
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
  border: 1px solid #000000;
  height: 36px;
  width: 32px;
  border-radius: 3px;
  background: #ffffff;
  cursor: pointer;
  -webkit-appearance: none;
  margin-top: -14px;
}
input[type=range]:focus::-webkit-slider-runnable-track {
  background: #367ebd;
}
input[type=range]::-moz-range-track {
  height: 8.4px;
  cursor: pointer;
  animate: 0.2s;
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
  background: #3071a9;
  border-radius: 1.3px;
  border: 0.2px solid #010101;
}
input[type=range]::-moz-range-thumb {
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
  border: 1px solid #000000;
  height: 36px;
  width: 32px;
  border-radius: 3px;
  background: #ffffff;
  cursor: pointer;
}
	
)=====";







