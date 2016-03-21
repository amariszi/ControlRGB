var RGBtoHSV = function (r, g, b) {
if (arguments.length === 1) {
	g = r.g, b = r.b, r = r.r;
}
var max = Math.max(r, g, b), min = Math.min(r, g, b),
	d = max - min,
	h,
	s = (max === 0 ? 0 : d / max),
	v = max / 255;

switch (max) {
	case min: h = 0; break;
	case r: h = (g - b) + d * (g < b ? 6: 0); h /= 6 * d; break;
	case g: h = (b - r) + d * 2; h /= 6 * d; break;
	case b: h = (r - g) + d * 4; h /= 6 * d; break;
}

return {
	h: h,
	s: s,
	v: v
};
}

var HSVtoRGB = function (h, s, v) {
var r, g, b, i, f, p, q, t;
if (arguments.length === 1) {
	s = h.s, v = h.v, h = h.h;
}
i = Math.floor(h * 6);
f = h * 6 - i;
p = v * (1 - s);
q = v * (1 - f * s);
t = v * (1 - (1 - f) * s);
switch (i % 6) {
	case 0: r = v, g = t, b = p; break;
	case 1: r = q, g = v, b = p; break;
	case 2: r = p, g = v, b = t; break;
	case 3: r = p, g = q, b = v; break;
	case 4: r = t, g = p, b = v; break;
	case 5: r = v, g = p, b = q; break;
}
return {
	r: Math.round(r * 255),
	g: Math.round(g * 255),
	b: Math.round(b * 255)
};
}
var red_slider = document.getElementById("red_slider");    
var green_slider = document.getElementById("green_slider");    
var blue_slider = document.getElementById("blue_slider");  
var muestra_color = document.getElementById("muestra_color"); 

document.addEventListener("DOMContentLoaded", function(event) { 
var sock; 

var conectar = function(){
	sock = new WebSocket("ws://192.168.4.1:1234", ['mensaje_vortex']);     
	sock.onerror = function(err){
		console.log('socket tiró error' + err);
	};
	sock.onclose = function(){
		console.log('socket desconectado');
		conectar();
	};
	sock.onopen = function(){ 
		console.log('socket conectado');
	};
	sock.onmessage = function(m){
		console.log("conector recibe mensaje por socket:", m.data);
		var valor = m.data.substring(1, m.data.length);	
		if(m.data.substring(0,1) == "R") {
			red_slider.value = valor;
			ultimo_red = valor;
		}
		if(m.data.substring(0,1) == "G") {
			green_slider.value = valor;
			ultimo_green = valor;
		}
		if(m.data.substring(0,1) == "B") {
			blue_slider.value = valor;
			ultimo_blue = valor;
		}
		mostrarColor();		            
	}                    
};

conectar();

 

var ultimo_red = -1;
var ultimo_green = -1;
var ultimo_blue = -1;

var mostrarColor = function(canal){
	var hsv = RGBtoHSV(ultimo_red, ultimo_green, ultimo_blue);
	hsv.h = 0.5 + hsv.h;
	hsv.s = 0.5 + hsv.h;
	hsv.v = 0.5 + hsv.h;
	var rgb = HSVtoRGB(hsv);   
	muestra_color.style="background-color:rgb(" + ultimo_red + "," + ultimo_green + "," + ultimo_blue + ");" + 
			" color:rgb(" + rgb.r + "," + rgb.g + "," + rgb.b + ");";
	muestra_color.innerHTML = "R" + ultimo_red + " G" + ultimo_green + " B" + ultimo_blue;                
};

setInterval(function(){
	if(red_slider.value != ultimo_red){
		ultimo_red = parseInt(red_slider.value);
		console.log("red:" + ultimo_red);
		mostrarColor();
		//var mensaje = {color:"R", valor:ultimo_red};
		if(sock.readyState == 1) sock.send("R"+ultimo_red);
	}
	if(green_slider.value != ultimo_green){
		ultimo_green = parseInt(green_slider.value);
		console.log("green:" + ultimo_green);
		mostrarColor();
		//var mensaje = {color:"G", valor:ultimo_green};
		if(sock.readyState == 1) sock.send("G"+ultimo_green);
	}
	if(blue_slider.value != ultimo_blue){
		ultimo_blue = parseInt(blue_slider.value);
		console.log("blue:" + ultimo_blue);
		mostrarColor();
		//var mensaje = {color:"V", valor:ultimo_blue};
		if(sock.readyState == 1) sock.send("B"+ultimo_blue);
	}
}, 200);                    
});
        