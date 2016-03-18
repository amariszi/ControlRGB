const char PAGE_index_html[] PROGMEM = R"=====(
 <!DOCTYPE html>
  <html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">    
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="index.css">

        <title>Control RGB</title>
    </head>
    <body>
        <div id="barra_superior">
            <div id="logo">
                
            </div>
            <div id="marca">
                AMARI SZI
            </div>
        </div>
    <input class="slider" id="red_slider" type="range" min="0" max="255" step="1" />    
    <input class="slider" id="green_slider" type="range" min="0" max="255" step="1" />    
    <input class="slider" id="blue_slider" type="range" min="0" max="255" step="1" />   
        <div id="muestra_color">
            
        </div>
    </body>    
  <script type="text/javascript" src="index.js"></script>     
</html>

)=====";
