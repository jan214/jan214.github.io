var cvs = document.getElementById("cvs");
var gl = cvs.getContext("webgl"); //{ preserveDrawingBuffer: true }
var rect = cvs.getBoundingClientRect();

var cwidth = document.getElementById("cvs").width;
var cheight = document.getElementById("cvs").height;

var bmousedown = false;

//main framebuffer vertex shader
var vText = "attribute vec2 position;"+
			"attribute vec2 texcoord;"+
			"varying vec2 atexcoord;"+
			  "void main(){"+
			  "atexcoord = texcoord;"+
			  "gl_Position = vec4(position.xy,0.0,1.0);"+
			  "}";

var vShader = gl.createShader(gl.VERTEX_SHADER);
gl.shaderSource(vShader,vText);
gl.compileShader(vShader);

if (!gl.getShaderParameter(vShader, gl.COMPILE_STATUS)) {
    alert(gl.getShaderInfoLog(vShader));
}

//main framebuffer fragment shader
var fText = "precision mediump float;"+
			"varying vec2 atexcoord;"+
			"uniform sampler2D sampler;"+
			"uniform sampler2D backtexture;"+
			"vec4 col;"+
			"void main(){"+
			//"gl_FragColor = vec4(atexcoord.x,atexcoord.y,0.0,1.0);"+
			"col = vec4(texture2D(backtexture,atexcoord).xyz,1.0);"+
			"if(texture2D(sampler,atexcoord).w > 0.0)"+
			"col = vec4(vec3(1.0,1.0,1.0) - texture2D(backtexture,atexcoord).xyz,1.0);"+
			//"gl_FragColor = texture2D(backtexture,atexcoord);"+
			//"gl_FragColor = texture2D(sampler,atexcoord);"+
			"gl_FragColor = col;"+
			"}";

var fShader = gl.createShader(gl.FRAGMENT_SHADER);
gl.shaderSource(fShader,fText);
gl.compileShader(fShader);

if (!gl.getShaderParameter(fShader, gl.COMPILE_STATUS)) {
    alert(gl.getShaderInfoLog(fShader));
}

//main framebuffer program
var program = gl.createProgram();
gl.attachShader(program,vShader);
gl.attachShader(program,fShader);
gl.linkProgram(program);
gl.useProgram(program);

var vertices = new Float32Array([-1.0,-1.0,
									1.0,-1.0,
									-1.0,1.0,
									1.0,1.0,
									1.0,-1.0,
									-1.0,1.0]);

var texcordinates = new Float32Array([0.0,0.0,
										1.0,0.0,
										0.0,1.0,
										1.0,1.0,
										1.0,0.0,
										0.0,1.0]);


var vbuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER,vbuffer);
gl.bufferData(gl.ARRAY_BUFFER,vertices,gl.STATIC_DRAW);

var positionLoc = gl.getAttribLocation(program,"position");
gl.vertexAttribPointer(positionLoc,2,gl.FLOAT,false,0,0);
gl.enableVertexAttribArray(positionLoc);

var tcbuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER,tcbuffer);
gl.bufferData(gl.ARRAY_BUFFER,texcordinates,gl.STATIC_DRAW);

var texcoordLoc = gl.getAttribLocation(program,"texcoord");
gl.vertexAttribPointer(texcoordLoc,2,gl.FLOAT,false,0,0);
gl.enableVertexAttribArray(texcoordLoc);

/*----------------------------------------------------------------------
/	create texture for showing the letters
/-----------------------------------------------------------------------*/
var atexture = gl.createTexture();
//gl.activeTexture(gl.TEXTURE0);
gl.bindTexture(gl.TEXTURE_2D,atexture);

var image = new Image();
image.src = "Hiragana/HaHi.png";

gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

//gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 1, 1, 0, gl.RGBA, gl.UNSIGNED_BYTE,new Uint8Array([0,255,0,255]));
gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);

image.addEventListener('load',function(){
	gl.bindTexture(gl.TEXTURE_2D,atexture);

	gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,image);
});

var texturesampler = gl.getUniformLocation(program, "sampler");
//gl.uniform1i(texturesampler,0);


/*-------------------------------------------------------------
/	Texture for drawing in a back framebuffer
/--------------------------------------------------------------*/
var backBufferTexture = gl.createTexture();
//gl.activeTexture(gl.TEXTURE0 + 1);
gl.bindTexture(gl.TEXTURE_2D,backBufferTexture);

gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, cwidth, cheight, 0, gl.RGBA, gl.UNSIGNED_BYTE,null);

var backTexturesampler = gl.getUniformLocation(program, "backtexture");
//gl.uniform1i(backTexturesampler,1);

/*-------------------------------------------------------
/	Create a Framebuffer
/--------------------------------------------------------*/
var backframeBuffer = gl.createFramebuffer();
gl.bindFramebuffer(gl.FRAMEBUFFER,backframeBuffer);

gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
gl.clearColor(0.0,0.0,0.0,1.0);

//attach the created texture to the color attachment of the framebuffer
var attachmentPoint = gl.COLOR_ATTACHMENT0;
gl.framebufferTexture2D(gl.FRAMEBUFFER,attachmentPoint,gl.TEXTURE_2D,backBufferTexture,0);

gl.bindFramebuffer(gl.FRAMEBUFFER,null);

//vertex shader for back framebuffer
var fvText = "attribute vec2 position;"+
			"uniform vec2 csize;"+
			"uniform float xoffset;"+
			"uniform float yoffset;"+
			"vec2 aposition;"+
			  "void main(){"+
			  "aposition = vec2((xoffset/csize.x -0.5) * 2.0 + (position.x/csize.x),(-yoffset/csize.y + 0.5) * 2.0 + (position.y/csize.y));"+
			  "gl_Position = vec4(aposition.xy,0.0,1.0);"+
			  "}";

var fvShader = gl.createShader(gl.VERTEX_SHADER);
gl.shaderSource(fvShader,fvText);
gl.compileShader(fvShader);

if (!gl.getShaderParameter(fvShader, gl.COMPILE_STATUS)) {
    alert(gl.getShaderInfoLog(fvShader));
}

//fragment shader for back framebuffer
var ffText = "precision mediump float;"+
				"void main(){"+
			  	"gl_FragColor = vec4(1.0,0.0,0.0,1.0);"+
			  	"}";

var ffShader = gl.createShader(gl.FRAGMENT_SHADER);
gl.shaderSource(ffShader,ffText);
gl.compileShader(ffShader);

if (!gl.getShaderParameter(ffShader, gl.COMPILE_STATUS)) {
    alert(gl.getShaderInfoLog(ffShader));
}

//back framebuffer program
var fprogram = gl.createProgram();
gl.attachShader(fprogram,fvShader);
gl.attachShader(fprogram,ffShader);
gl.linkProgram(fprogram);
gl.useProgram(fprogram);

var fvertices = new Float32Array([-5.0,5.0,
				5.0,5.0,
				5.0,-5.0,
				-5.0,5.0,
				-5.0,-5.0,
				5.0,-5.0]);

var fvbuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER,fvbuffer);
gl.bufferData(gl.ARRAY_BUFFER,fvertices,gl.STATIC_DRAW);

var fpositionLoc = gl.getAttribLocation(fprogram,"position");
gl.vertexAttribPointer(fpositionLoc,2,gl.FLOAT,false,0,0);
gl.enableVertexAttribArray(fpositionLoc);

/*---------------------------------------------
/		uniforms for back framebuffer
/----------------------------------------------*/
var canvasx = gl.getUniformLocation(fprogram,"csize");
gl.uniform2f(canvasx,cwidth,cheight);

var uniformx = gl.getUniformLocation(fprogram,"xoffset");
gl.uniform1f(uniformx,1.0);
var uniformy = gl.getUniformLocation(fprogram,"yoffset");
gl.uniform1f(uniformy,1.0);

//gl.bindFramebuffer(gl.FRAMEBUFFER,null);

function clearBackBufferTexture(){
	gl.bindFramebuffer(gl.FRAMEBUFFER,backframeBuffer);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	gl.clearColor(0.0,0.0,0.0,1.0);
	gl.bindFramebuffer(gl.FRAMEBUFFER,null);
}

/*-----------------------------------------------------------------
/        Buttons, alphabets and letters setup
/------------------------------------------------------------------*/

var mousex = 0.0;
var mousey = 0.0;

var nextal = document.getElementById("nextal");
var previousal = document.getElementById("previousal");
var nextle = document.getElementById("next");
var previousle = document.getElementById("previous");
var alphabet = document.getElementById("alphabet");
var letter = document.getElementById("letter");

var alphabetn = 0;
var lettern = 0;

//Hiragana and Kanji working Katakana needs to be implemented yet
var alphabets = ["Hiragana","Katakana","Kanji"];
var letters = ["HaHi","BaHi","PaHi","HiHi","BiHi","PiHi","HeHi","BeHi","PeHi","TaHi","DaHi","AHi","BaHi","BeHi","BiHi","BoHi","BuHi","ChiHi","DaHi","DeHi","DoHi","DuHi","EHi","FuHi","GaHi","GeHi","GiHi","GoHi","GuHi","HaHi","HeHi","HiHi","HoHi","IHi","JiHi","KaHi","KeHi","KiHi","KoHi","KuHi","MaHi","MeHi","MiHi","MoHi","MuHi","NaHi","NeHi","NHi","NiHi","NoHi","NuHi","OHi","PaHi","PeHi","PiHi","PoHi","PuHi","RaHi","ReHi","RiHi","RoHi","RuHi","SaHi","SeHi","ShiHi","SoHi","SuHi","TaHi","TeHi","ToHi","TsuHi","UHi","WaHi","WeHi","WiHi","WoHi","YaHi","YoHi","ZuHi","YuHi","ZaHi","ZeHi","ZoHi"];
var words = ["WaTaShi","Kanojo","KaReRa","BoKu","AMaI"];

/*----------------------------------------------------------------------
/	click implementation for Buttons
/-----------------------------------------------------------------------*/
nextal.addEventListener("click",function(){
	alphabetn++;
	lettern = 0;
	if(alphabetn > 2){
		alphabetn = 0;
	}
	alphabet.text = alphabets[alphabetn];
});

previousal.addEventListener("click",function(){
	alphabetn--;
	lettern = 0;
	if(alphabetn < 0){
		alphabetn = 2;
	}
	alphabet.text = alphabets[alphabetn];
});

previousle.addEventListener("click",function(){
	clearBackBufferTexture();

	gl.bindTexture(gl.TEXTURE_2D,atexture);
	lettern--;

	if(alphabetn == 2){
		if(lettern < 0){
			lettern = (words.length-1);
		}
		image.src = alphabets[2]+"/"+words[lettern]+".png";
		letter.text = words[lettern];
	}else{
		if(lettern < 0){
			lettern = (letters.length-1);
		}
		image.src = alphabets[alphabetn]+"/"+letters[lettern]+".png";
		letter.text = letters[lettern];
	}

	gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,image);
});

nextle.addEventListener("click",function(){
	clearBackBufferTexture();

	gl.bindTexture(gl.TEXTURE_2D,atexture);
	lettern++;
	if(alphabetn == 2){
		if(lettern > (words.length-1)){
			lettern = 0;
		}
		image.src = alphabets[2]+"/"+words[lettern]+".png";
		letter.text = words[lettern];
	}else{
		if(lettern > (letters.length-1)){
			lettern = 0;
		}
		image.src = alphabets[alphabetn]+"/"+letters[lettern]+".png";
		letter.text = letters[lettern];
	}

	gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,image);
});

/*---------------------------------------------------------------------------------
/	Mouse and Touch events
/----------------------------------------------------------------------------------*/

cvs.addEventListener('touchstart', event => {
	event.preventDefault();
	bmousedown = true;
	mousex = event.pageX-rect.left;
	mousey = event.pageY-rect.top;
});

cvs.addEventListener('touchmove', event => {
	if(bmousedown){
		mousex = event.pageX-rect.left;
		mousey = event.pageY-rect.top;
	}
});

cvs.addEventListener('touchend', event =>{
	bmousedown = false;
});

cvs.addEventListener('mousedown', event => {
	console.log("down:"+(event.pageX-rect.left)+"/"+(event.pageY-rect.top));
	bmousedown = true;
	mousex = event.pageX-rect.left;
	mousey = event.pageY-rect.top;
});

cvs.addEventListener('mouseup', event => {
	bmousedown = false;
});

/*-------------------------------------------------------------
/		mousemve and render
/--------------------------------------------------------------*/

cvs.addEventListener('mousemove', event => {
	if(bmousedown){
		mousex = event.pageX-rect.left;
		mousey = event.pageY-rect.top;
	}
});

gl.useProgram(program);

function loop(){
	if(bmousedown){
		//gl.bindTexture(gl.TEXTURE_2D,backBufferTexture);

		//setup back framebuffer and back texture for drawing
		gl.useProgram(fprogram);
		gl.bindFramebuffer(gl.FRAMEBUFFER,backframeBuffer);
		gl.bindBuffer(gl.ARRAY_BUFFER,fvbuffer);
		gl.vertexAttribPointer(fpositionLoc,2,gl.FLOAT,false,0,0);

		gl.uniform1f(uniformx,mousex);
		gl.uniform1f(uniformy,mousey);

		gl.drawArrays(gl.TRIANGLES,0,6);

		//unbind back framebuffer and use front framebuffer
		gl.bindFramebuffer(gl.FRAMEBUFFER,null);
		gl.useProgram(program);
	}

		gl.bindBuffer(gl.ARRAY_BUFFER,vbuffer);
		gl.vertexAttribPointer(positionLoc,2,gl.FLOAT,false,0,0);

		gl.bindBuffer(gl.ARRAY_BUFFER,tcbuffer);
		gl.vertexAttribPointer(texcoordLoc,2,gl.FLOAT,false,0,0);

		gl.uniform1i(texturesampler,0);
		gl.activeTexture(gl.TEXTURE0);
		gl.bindTexture(gl.TEXTURE_2D,atexture);

		gl.uniform1i(backTexturesampler,1);
		gl.activeTexture(gl.TEXTURE1);
		gl.bindTexture(gl.TEXTURE_2D,backBufferTexture);

		gl.drawArrays(gl.TRIANGLES,0,6);

	window.requestAnimationFrame(loop);
};

window.requestAnimationFrame(loop);