const cvs = document.getElementById("canvas");
const gl = cvs.getContext('webgl');

const canvaswidth = cvs.width;
const canvasheight = cvs.height;

var verCode = "attribute vec3 aposition;"+
			  "vec3 aaposition;"+
			  "attribute vec2 atexture;"+
			  "varying vec2 texture;"+
			  "attribute vec3 anormal;"+
			  "vec3 aanormal;"+
			  "attribute vec3 acolor;"+
			  "varying vec3 nanormal;"+
			  "varying vec3 frag;"+
			  "varying vec3 color;"+
			  "uniform mat3 matrix;"+
			  "uniform float translate;"+
			  "uniform float translatex;"+
			  "uniform float translatey;"+
			  "uniform mat3 rotationx;"+
			  "uniform mat3 rotationy;"+
			  "mat3 rotation;"+
			  "void main(){"+
			  "texture = atexture;"+
			  "color = acolor;"+
			  "rotation = rotationy * rotationx;"+
			  "aaposition = aposition * rotation;"+
			  "nanormal = normalize(anormal * rotation);"+
			  "aaposition = aaposition + vec3(translatex,translatey,translate);"+
			  "frag = aaposition;"+
			  "aaposition = aaposition * matrix;"+
			  "gl_Position = vec4(aaposition,1.0 + (aaposition.z + translate));"+
			  "}";

var vertShader = gl.createShader(gl.VERTEX_SHADER);
gl.shaderSource(vertShader,verCode);
gl.compileShader(vertShader);

if (!gl.getShaderParameter(vertShader, gl.COMPILE_STATUS)) {
    alert(gl.getShaderInfoLog(vertShader));
}

var fragCode = "precision mediump float;"+
			   "varying vec2 texture;"+
			   "uniform sampler2D sampler;"+
			   "vec3 texcolor;"+
			   "varying vec3 color;"+
			   "vec3 id;"+
			   "varying vec3 nanormal;"+
			   "float ambient;"+
			   "vec3 camvert;"+
			   "varying vec3 frag;"+
			   "vec3 specular;"+
			   "float specularval;"+
			   "float bright;"+
			   "void main(){"+
			   "id = normalize(vec3(0.0,0.0,0.0)-frag);"+
			   "ambient = max(dot(id,nanormal),0.1);"+
			   "camvert = normalize(vec3(0.0,0.0,0.0)-frag);"+
			   "specular = normalize(reflect(-id,nanormal));"+
			   "specularval = max(pow(dot(camvert,specular),1.0),0.1);"+
			   "texcolor = texture2D(sampler,texture).xyz;"+
			   "gl_FragColor = vec4(texcolor*ambient+vec3(1.0,1.0,1.0)*specularval,1.0);"+
			   "}";

var fragShader = gl.createShader(gl.FRAGMENT_SHADER);
gl.shaderSource(fragShader,fragCode);
gl.compileShader(fragShader);

if (!gl.getShaderParameter(fragShader, gl.COMPILE_STATUS)) {
    alert(gl.getShaderInfoLog(fragShader));
}

var shaderProgram = gl.createProgram();
gl.attachShader(shaderProgram,vertShader);
gl.attachShader(shaderProgram,fragShader);
gl.linkProgram(shaderProgram);
gl.useProgram(shaderProgram);

var fov = 120;

var matrix = new Float32Array([
								1/Math.sin((fov/2)*(Math.PI/180)),0,0,
								0,1/Math.sin((((fov/2)/16)*9)*(Math.PI/180)),0,
								0,0,(1.0/100.0)
								]);

var threedtriangle = new Float32Array([
	1.000000,-1.000000,-1.000000,
	1.000000,-1.000000,1.000000,
	-1.000000,-1.000000,1.000000,
	-1.000000,-1.000000,-1.000000,
	1.000000,1.000000,-0.999999,
	0.999999,1.000000,1.000001,
	-1.000000,1.000000,1.000000,
	-1.000000,1.000000,-1.000000
]);

var indices = new Uint16Array([
	0,2,3,
	7,5,4,
	4,1,0,
	5,2,1,
	2,7,3,
	0,7,4,
	0,1,2,
	7,6,5,
	4,5,1,
	5,6,2,
	2,6,7,
	0,3,7
]);

var nIndices = new Uint16Array([
	0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5
]);

var threedtrianglenormals = new Float32Array([
	0.0000,-1.0000,0.0000,
	0.0000,1.0000,0.0000,
	1.0000,0.0000,0.0000,
	-0.0000,-0.0000,1.0000,
	-1.0000,-0.0000,-0.0000,
	0.0000,0.0000,-1.0000
]);

var objloader;
var fbxloader;
var indiceslength;
var xhrobj;
var xhrtex;

document.getElementById("nyc").addEventListener("click",function(){
	document.getElementById("loading").innerHTML = "<a style='color:rgb(255, 64, 0)'>Loading ...</a>";
	translate = 0.0;
	translatex = 0.0;
	translatey = 0.0;
	rotationx = 0.0;
	rotationy = 0.0;
	rotationz = 0.0;

	document.getElementById("tech").innerHTML = "<img class='techlogos' src='blender.png' alt='blender' title='Blender'><img id='texture' class='techlogos' alt='texture' title='Texture'></img>";

	xhrobj = new XMLHttpRequest();
	xhrtex = new XMLHttpRequest();
	xhrtex.responseType = "arraybuffer";
	xhrobj.responseType = "text";

	xhrtex.onload = function(e){
		if (this.status == 200){
			var uInt8Array = new Uint8Array(this.response);
			var i = uInt8Array.length;
			var biStr = new Array(i);
			while (i--){
				biStr[i] = String.fromCharCode(uInt8Array[i]);
			}
			var data = biStr.join('');
			var base64 = window.btoa(data);

			document.getElementById("texture").src = "data:image/png;base64,"+base64;
			texture = gl.createTexture();
			gl.bindTexture(gl.TEXTURE_2D,texture);

			gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 1, 1, 0, gl.RGBA, gl.UNSIGNED_BYTE,new Uint8Array([255, 255, 255, 255]));
		}
	}

	xhrtex.open("GET","default.png",true);
	xhrtex.send();

	xhrobj.onload = function(e){
		objloader = new OBJLoader(xhrobj.responseText);

		console.log(objloader.getIndices().length+" "+objloader.getVertices().length+" "+objloader.getTextures().length);

		indexBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,indexBuffer);
		gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,new Uint16Array(objloader.getIndices()),gl.STATIC_DRAW);
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,null);

		vertexBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,vertexBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getVertices()),gl.STATIC_DRAW);
		positionAtrib = gl.getAttribLocation(shaderProgram,"aposition");
		gl.vertexAttribPointer(positionAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		textureBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,textureBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getTextures()),gl.STATIC_DRAW);
		textureAtrib = gl.getAttribLocation(shaderProgram,"atexture");
		gl.vertexAttribPointer(textureAtrib,2,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		colorBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,colorBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getColors()),gl.STATIC_DRAW);
		colorAtrib = gl.getAttribLocation(shaderProgram,"acolor");
		gl.vertexAttribPointer(colorAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		normalBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,normalBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getNormals()),gl.STATIC_DRAW);
		normalAtrib = gl.getAttribLocation(shaderProgram,"anormal");
		gl.vertexAttribPointer(normalAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		indiceslength = objloader.getIndices().length;

		document.getElementById("loading").innerHTML = "<a style='color:rgb(0, 166, 61)'>Model Loaded</a>";
	}

	xhrobj.open("GET","newyork.obj",true);
	xhrobj.send();
});

document.getElementById("la").addEventListener("click",function(){
	document.getElementById("loading").innerHTML = "<a style='color:rgb(255, 64, 0)'>Loading ...</a>";
	translate = 0.0;
	translatex = 0.0;
	translatey = 0.0;
	rotationx = 0.0;
	rotationy = 0.0;
	rotationz = 0.0;

	document.getElementById("tech").innerHTML = "<img class='techlogos' src='blender.png' alt='blender' title='Blender'><img id='texture' class='techlogos' alt='texture' title='Texture'></img>";

	xhrobj = new XMLHttpRequest();
	xhrtex = new XMLHttpRequest();
	xhrtex.responseType = "arraybuffer";
	xhrobj.responseType = "text";

	xhrtex.onload = function(e){
		if (this.status == 200){
			var uInt8Array = new Uint8Array(this.response);
			var i = uInt8Array.length;
			var biStr = new Array(i);
			while (i--){
				biStr[i] = String.fromCharCode(uInt8Array[i]);
			}
			var data = biStr.join('');
			var base64 = window.btoa(data);

			document.getElementById("texture").src = "data:image/png;base64,"+base64;
			texture = gl.createTexture();
			gl.bindTexture(gl.TEXTURE_2D,texture);

			gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 1, 1, 0, gl.RGBA, gl.UNSIGNED_BYTE,new Uint8Array([255, 255, 255, 255]));
		}
	}

	xhrtex.open("GET","default.png",true);
	xhrtex.send();

	xhrobj.onload = function(e){
		objloader = new OBJLoader(xhrobj.responseText);

		console.log(objloader.getIndices().length+" "+objloader.getVertices().length+" "+objloader.getTextures().length);

		indexBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,indexBuffer);
		gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,new Uint16Array(objloader.getIndices()),gl.STATIC_DRAW);
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,null);

		vertexBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,vertexBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getVertices()),gl.STATIC_DRAW);
		positionAtrib = gl.getAttribLocation(shaderProgram,"aposition");
		gl.vertexAttribPointer(positionAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		textureBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,textureBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getTextures()),gl.STATIC_DRAW);
		textureAtrib = gl.getAttribLocation(shaderProgram,"atexture");
		gl.vertexAttribPointer(textureAtrib,2,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		colorBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,colorBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getColors()),gl.STATIC_DRAW);
		colorAtrib = gl.getAttribLocation(shaderProgram,"acolor");
		gl.vertexAttribPointer(colorAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		normalBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,normalBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getNormals()),gl.STATIC_DRAW);
		normalAtrib = gl.getAttribLocation(shaderProgram,"anormal");
		gl.vertexAttribPointer(normalAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		indiceslength = objloader.getIndices().length;
		document.getElementById("loading").innerHTML = "<a style='color:rgb(0, 166, 61)'>Model Loaded</a>";
	}

	xhrobj.open("GET","losangeles.obj",true);
	xhrobj.send();
});

document.getElementById("basketball2").addEventListener("click",function(){
	document.getElementById("loading").innerHTML = "<a style='color:rgb(255, 64, 0)'>Loading ...</a>";
	translate = 0.0;
	translatex = 0.0;
	translatey = 0.0;
	rotationx = 0.0;
	rotationy = 0.0;
	rotationz = 0.0;

	document.getElementById("tech").innerHTML = "<img class='techlogos' src='maya.png' alt='maya' title='Maya'><img id='texture' class='techlogos' alt='texture' title='Texture'></img>";

	xhrobj = new XMLHttpRequest();
	xhrtex = new XMLHttpRequest();
	xhrtex.responseType = "arraybuffer";
	xhrobj.responseType = "text";

	xhrtex.onload = function(e){
		if (this.status == 200){
			var uInt8Array = new Uint8Array(this.response);
			var i = uInt8Array.length;
			var biStr = new Array(i);
			while (i--){
				biStr[i] = String.fromCharCode(uInt8Array[i]);
			}
			var data = biStr.join('');
			var base64 = window.btoa(data);

			document.getElementById("texture").src = "data:image/png;base64,"+base64;
			document.getElementById("texture").onload = function(e){
				texture = gl.createTexture();
				gl.bindTexture(gl.TEXTURE_2D,texture);

				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

				gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,document.getElementById("texture"));

				gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
				gl.activeTexture(gl.TEXTURE0);
			}
		}
	}

	xhrtex.open("GET","deandrejordantexture.png",true);
	xhrtex.send();

	xhrobj.onload = function(e){
		objloader = new OBJLoader(xhrobj.responseText);

		console.log(objloader.getIndices().length+" "+objloader.getVertices().length+" "+objloader.getTextures().length);

		indexBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,indexBuffer);
		gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,new Uint16Array(objloader.getIndices()),gl.STATIC_DRAW);
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,null);

		vertexBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,vertexBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getVertices()),gl.STATIC_DRAW);
		positionAtrib = gl.getAttribLocation(shaderProgram,"aposition");
		gl.vertexAttribPointer(positionAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		textureBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,textureBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getTextures()),gl.STATIC_DRAW);
		textureAtrib = gl.getAttribLocation(shaderProgram,"atexture");
		gl.vertexAttribPointer(textureAtrib,2,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		colorBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,colorBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getColors()),gl.STATIC_DRAW);
		colorAtrib = gl.getAttribLocation(shaderProgram,"acolor");
		gl.vertexAttribPointer(colorAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		normalBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,normalBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getNormals()),gl.STATIC_DRAW);
		normalAtrib = gl.getAttribLocation(shaderProgram,"anormal");
		gl.vertexAttribPointer(normalAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		indiceslength = objloader.getIndices().length;
		document.getElementById("loading").innerHTML = "<a style='color:rgb(0, 166, 61)'>Model Loaded</a>";
	}

	xhrobj.open("GET","deandrejordan.obj",true);
	xhrobj.send();
});

document.getElementById("basketball").addEventListener("click",function(){
	document.getElementById("loading").innerHTML = "<a style='color:rgb(255, 64, 0)'>Loading ...</a>";
	translate = 0.0;
	translatex = 0.0;
	translatey = 0.0;
	rotationx = 0.0;
	rotationy = 0.0;
	rotationz = 0.0;

	document.getElementById("tech").innerHTML = "<img class='techlogos' src='blender.png' alt='blender' title='Blender'><img id='texture' class='techlogos' alt='texture' title='Texture'></img>";

	xhrobj = new XMLHttpRequest();
	xhrtex = new XMLHttpRequest();
	xhrtex.responseType = "arraybuffer";
	xhrobj.responseType = "text";

	xhrtex.onload = function(e){
		if (this.status == 200){
			var uInt8Array = new Uint8Array(this.response);
			var i = uInt8Array.length;
			var biStr = new Array(i);
			while (i--){
				biStr[i] = String.fromCharCode(uInt8Array[i]);
			}
			var data = biStr.join('');
			var base64 = window.btoa(data);

			document.getElementById("texture").src = "data:image/png;base64,"+base64;
			document.getElementById("texture").onload = function(e){
				texture = gl.createTexture();
				gl.bindTexture(gl.TEXTURE_2D,texture);

				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

				gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,document.getElementById("texture"));

				gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
				gl.activeTexture(gl.TEXTURE0);
			}
		}
	}

	xhrtex.open("GET","basketballplayer1texture.png",true);
	xhrtex.send();

	xhrobj.onload = function(e){
		objloader = new OBJLoader(xhrobj.responseText);

		console.log(objloader.getIndices().length+" "+objloader.getVertices().length+" "+objloader.getTextures().length);

		indexBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,indexBuffer);
		gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,new Uint16Array(objloader.getIndices()),gl.STATIC_DRAW);
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,null);

		vertexBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,vertexBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getVertices()),gl.STATIC_DRAW);
		positionAtrib = gl.getAttribLocation(shaderProgram,"aposition");
		gl.vertexAttribPointer(positionAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		textureBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,textureBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getTextures()),gl.STATIC_DRAW);
		textureAtrib = gl.getAttribLocation(shaderProgram,"atexture");
		gl.vertexAttribPointer(textureAtrib,2,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		colorBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,colorBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getColors()),gl.STATIC_DRAW);
		colorAtrib = gl.getAttribLocation(shaderProgram,"acolor");
		gl.vertexAttribPointer(colorAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		normalBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,normalBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getNormals()),gl.STATIC_DRAW);
		normalAtrib = gl.getAttribLocation(shaderProgram,"anormal");
		gl.vertexAttribPointer(normalAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		indiceslength = objloader.getIndices().length;
		document.getElementById("loading").innerHTML = "<a style='color:rgb(0, 166, 61)'>Model Loaded</a>";
	}

	xhrobj.open("GET","basketballplayer1.obj",true);
	xhrobj.send();
});

document.getElementById("tombraider").addEventListener("click",function(){
	document.getElementById("loading").innerHTML = "<a style='color:rgb(255, 64, 0)'>Loading ...</a>";
	translate = 0.0;
	translatex = 0.0;
	translatey = 0.0;
	rotationx = 0.0;
	rotationy = 0.0;
	rotationz = 0.0;

	document.getElementById("tech").innerHTML = "<img class='techlogos' src='blender.png' alt='blender' title='Blender'><img id='texture' class='techlogos' alt='texture' title='Texture'></img>";

	xhrobj = new XMLHttpRequest();
	xhrtex = new XMLHttpRequest();
	xhrtex.responseType = "arraybuffer";
	xhrobj.responseType = "text";

	xhrtex.onload = function(e){
		if (this.status == 200){
			var uInt8Array = new Uint8Array(this.response);
			var i = uInt8Array.length;
			var biStr = new Array(i);
			while (i--){
				biStr[i] = String.fromCharCode(uInt8Array[i]);
			}
			var data = biStr.join('');
			var base64 = window.btoa(data);

			document.getElementById("texture").src = "data:image/png;base64,"+base64;
			document.getElementById("texture").onload = function(e){
				texture = gl.createTexture();
				gl.bindTexture(gl.TEXTURE_2D,texture);

				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

				gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,document.getElementById("texture"));

				gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
				gl.activeTexture(gl.TEXTURE0);
			}
		}
	}

	xhrtex.open("GET","tombraidertexture.png",true);
	xhrtex.send();

	xhrobj.onload = function(e){
		objloader = new OBJLoader(xhrobj.responseText);

		console.log(objloader.getIndices().length+" "+objloader.getVertices().length+" "+objloader.getTextures().length);

		indexBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,indexBuffer);
		gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,new Uint16Array(objloader.getIndices()),gl.STATIC_DRAW);
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,null);

		vertexBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,vertexBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getVertices()),gl.STATIC_DRAW);
		positionAtrib = gl.getAttribLocation(shaderProgram,"aposition");
		gl.vertexAttribPointer(positionAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		textureBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,textureBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getTextures()),gl.STATIC_DRAW);
		textureAtrib = gl.getAttribLocation(shaderProgram,"atexture");
		gl.vertexAttribPointer(textureAtrib,2,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		colorBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,colorBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getColors()),gl.STATIC_DRAW);
		colorAtrib = gl.getAttribLocation(shaderProgram,"acolor");
		gl.vertexAttribPointer(colorAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		normalBuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,normalBuffer);
		gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getNormals()),gl.STATIC_DRAW);
		normalAtrib = gl.getAttribLocation(shaderProgram,"anormal");
		gl.vertexAttribPointer(normalAtrib,3,gl.FLOAT,false,0,0);
		gl.bindBuffer(gl.ARRAY_BUFFER,null);

		indiceslength = objloader.getIndices().length;
		document.getElementById("loading").innerHTML = "<a style='color:rgb(0, 166, 61)'>Model Loaded</a>";
	}

	xhrobj.open("GET","tombraider.obj",true);
	xhrobj.send();
});

	var openfile = function(event){
		var input = event.target;
		console.log(input.files[0].name);
		var text;
		var fr = new FileReader();
		
		fr.onload = function(){
			text = fr.result;
			if(input.files[0].name.includes(".obj")){
				objloader = new OBJLoader(text);

				texture = gl.createTexture();
				gl.bindTexture(gl.TEXTURE_2D,texture);

				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

				gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,document.getElementById("testtex"));

				gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
				gl.activeTexture(gl.TEXTURE0);

				indexBuffer = gl.createBuffer();
				gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,indexBuffer);
				gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,new Uint16Array(objloader.getIndices()),gl.STATIC_DRAW);
				gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,null);

				vertexBuffer = gl.createBuffer();
				gl.bindBuffer(gl.ARRAY_BUFFER,vertexBuffer);
				gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getVertices()),gl.STATIC_DRAW);
				positionAtrib = gl.getAttribLocation(shaderProgram,"aposition");
				gl.vertexAttribPointer(positionAtrib,3,gl.FLOAT,false,0,0);
				gl.bindBuffer(gl.ARRAY_BUFFER,null);

				colorBuffer = gl.createBuffer();
				gl.bindBuffer(gl.ARRAY_BUFFER,colorBuffer);
				gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getColors()),gl.STATIC_DRAW);
				colorAtrib = gl.getAttribLocation(shaderProgram,"acolor");
				gl.vertexAttribPointer(colorAtrib,3,gl.FLOAT,false,0,0);
				gl.bindBuffer(gl.ARRAY_BUFFER,null);

				normalBuffer = gl.createBuffer();
				gl.bindBuffer(gl.ARRAY_BUFFER,normalBuffer);
				gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(objloader.getNormals()),gl.STATIC_DRAW);
				normalAtrib = gl.getAttribLocation(shaderProgram,"anormal");
				gl.vertexAttribPointer(normalAtrib,3,gl.FLOAT,false,0,0);
				gl.bindBuffer(gl.ARRAY_BUFFER,null);

				indiceslength = objloader.getIndices().length;
			}else if(input.files[0].name.includes(".fbx")){
				fbxloader = new FBXLoader(text);

				indexBuffer = gl.createBuffer();
				gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,indexBuffer);
				gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,new Uint16Array(fbxloader.getIndices()),gl.STATIC_DRAW);
				gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,null);

				vertexBuffer = gl.createBuffer();
				gl.bindBuffer(gl.ARRAY_BUFFER,vertexBuffer);
				gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(fbxloader.getVertices()),gl.STATIC_DRAW);
				gl.bindBuffer(gl.ARRAY_BUFFER,null);

				colorBuffer = gl.createBuffer();
				gl.bindBuffer(gl.ARRAY_BUFFER,colorBuffer);
				gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(fbxloader.getColors()),gl.STATIC_DRAW);
				gl.bindBuffer(gl.ARRAY_BUFFER,null);

				normalBuffer = gl.createBuffer();
				gl.bindBuffer(gl.ARRAY_BUFFER,normalBuffer);
				gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(fbxloader.getNormals()),gl.STATIC_DRAW);
				gl.bindBuffer(gl.ARRAY_BUFFER,null);

				indiceslength = fbxloader.getIndices().length;
			}
	    };
	    
		fr.readAsText(input.files[0]);
	}

	var indicesreal = new Array();
	var threedtrianglereal = new Array();
	var normalsreal = new Array();
	var texturereal = new Array();
	var translate = 1.0;
	var translatex = 0.0;
	var translatey = 0.0;
	var rotationx = 0.0;
	var rotationy = 0.0;
	var rotationz = 0.0;
	var rot = 0.0;

	var rotatex = new Float32Array([
				1.0,0.0,0.0,
				0.0,Math.cos(rotationx*(Math.PI/180.0)),-Math.sin(rotationx*(Math.PI/180.0)),
				0.0,Math.sin(rotationx*(Math.PI/180.0)),Math.cos(rotationx*(Math.PI/180.0))
		]);
	var rotatey = new Float32Array([
				Math.cos(rotationy*(Math.PI/180)),0,Math.sin(rotationy*(Math.PI/180)),
				0,1,0,
				-Math.sin(rotationy*(Math.PI/180)),0,Math.cos(rotationy*(Math.PI/180))
		]);
	var rotatez = new Float32Array([
				Math.cos(rotationz*(Math.PI/180)),-Math.sin(rotationz*(Math.PI/180)),0,0,
				Math.sin(rotationz*(Math.PI/180)),Math.cos(rotationz*(Math.PI/180)),0,0,
				0,0,1,0,
				0,0,0,1
		]);

	for(var i = 0; i < indices.length; i++){
		threedtrianglereal.push(threedtriangle[indices[i]*3]);
		threedtrianglereal.push(threedtriangle[indices[i]*3+1]);
		threedtrianglereal.push(threedtriangle[indices[i]*3+2]);

		normalsreal.push(threedtrianglenormals[nIndices[i]*3]);
		normalsreal.push(threedtrianglenormals[nIndices[i]*3+1]);
		normalsreal.push(threedtrianglenormals[nIndices[i]*3+2]);

		texturereal.push(0.0);
		texturereal.push(0.0);
	}

	for(var i = 0; i < indices.length; i++){
		indicesreal.push(i);
	}

	indiceslength = indicesreal.length;

	var colors = new Float32Array([
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 1.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 1.0, 1.0
		]);

	console.log(indicesreal.length+"/"+threedtrianglereal.length+"/"+normalsreal.length+"/"+colors.length);

	document.getElementById("canvas").addEventListener("wheel",function(event){
		event.preventDefault();

		translate += event.deltaY*deltaTime;
	});

	document.getElementById("canvas").addEventListener("mousedown",function(event){
		if(event.button == 0){
			document.getElementById("canvas").onmousemove = function(event){
				if(rotationy > 360){
					rotationy = 0;
				}

				if(rotationy < 0){
					rotationy = 360;
				}

				rotationy -= event.movementX*50*deltaTime;

				rotatey = [
						Math.cos(rotationy*(Math.PI/180.0)),0.0,Math.sin(rotationy*(Math.PI/180.0)),
						0.0,1.0,0.0,
						-Math.sin(rotationy*(Math.PI/180.0)),0.0,Math.cos(rotationy*(Math.PI/180.0))
				];

				if(rotationx > 90){
					rotationx = 89.9;
				}

				if(rotationx < -90){
					rotationx = -89.9;
				}

				rotationx += -event.movementY*50*deltaTime;

				rotatex = [
						1.0,0.0,0.0,
						0.0,Math.cos(rotationx*(Math.PI/180.0)),-Math.sin(rotationx*(Math.PI/180.0)),
						0.0,Math.sin(rotationx*(Math.PI/180.0)),Math.cos(rotationx*(Math.PI/180.0))
				];
			}
		}else if(event.button == 2){
			document.getElementById("canvas").onmousemove = function(event){
				translatex += event.movementX*deltaTime;
				translatey -= event.movementY*deltaTime;
			}
		}
	});

	document.getElementById("canvas").addEventListener("mouseup",function(event){
		document.getElementById("canvas").onmousemove = null;
	});

	document.getElementById("canvas").oncontextmenu = function(){
		return false;
	};

	var matrixLoc = gl.getUniformLocation(shaderProgram,"matrix");
	gl.uniformMatrix3fv(matrixLoc,false,matrix);

	var translateLoc = gl.getUniformLocation(shaderProgram,"translate");
	var translatexLoc = gl.getUniformLocation(shaderProgram,"translatex");
	var translateyLoc = gl.getUniformLocation(shaderProgram,"translatey");
	var rotationxLoc = gl.getUniformLocation(shaderProgram,"rotationx");
	var rotationyLoc = gl.getUniformLocation(shaderProgram,"rotationy");

	var textureLoc = gl.getUniformLocation(shaderProgram, 'sampler');

	var texture = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D,texture);

	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 1, 1, 0, gl.RGBA, gl.UNSIGNED_BYTE,new Uint8Array([255, 255, 255, 255]));

	gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
	gl.activeTexture(gl.TEXTURE0);

	var indexBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,indexBuffer);
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,new Uint16Array(indicesreal),gl.STATIC_DRAW);
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,null);

	var vertexBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,vertexBuffer);
	gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(threedtrianglereal),gl.STATIC_DRAW);
	var positionAtrib = gl.getAttribLocation(shaderProgram,"aposition");
	gl.vertexAttribPointer(positionAtrib,3,gl.FLOAT,false,0,0);
	gl.enableVertexAttribArray(positionAtrib);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);

	var textureBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,textureBuffer);
	gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(new Float32Array(texturereal)),gl.STATIC_DRAW);
	var textureAtrib = gl.getAttribLocation(shaderProgram,"atexture");
	gl.vertexAttribPointer(textureAtrib,2,gl.FLOAT,false,0,0);
	gl.enableVertexAttribArray(textureAtrib);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);

	var colorBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,colorBuffer);
	gl.bufferData(gl.ARRAY_BUFFER,colors,gl.STATIC_DRAW);
	var colorAtrib = gl.getAttribLocation(shaderProgram,"acolor");
	gl.vertexAttribPointer(colorAtrib,3,gl.FLOAT,false,0,0);
	gl.enableVertexAttribArray(colorAtrib);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);

	var normalBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER,normalBuffer);
	gl.bufferData(gl.ARRAY_BUFFER,new Float32Array(normalsreal),gl.STATIC_DRAW);
	var normalAtrib = gl.getAttribLocation(shaderProgram,"anormal");
	gl.vertexAttribPointer(normalAtrib,3,gl.FLOAT,false,0,0);
	gl.enableVertexAttribArray(normalAtrib);
	gl.bindBuffer(gl.ARRAY_BUFFER,null);

	indiceslength = indicesreal.length;

	var deltaTime;
	var now = Date.now();

	gl.enable(gl.DEPTH_TEST);
	gl.enable(gl.CULL_FACE);
	gl.cullFace(gl.FRONT);

	function loop(){
		deltaTime = (Date.now() - now)*0.001;
		now = Date.now();

		gl.clearColor(0.0,0.0,0.0,1.0);
		gl.clear(gl.COLOR_BUFFER_BIT);

		gl.uniform1f(translateLoc,translate);
		gl.uniform1f(translatexLoc,translatex);
		gl.uniform1f(translateyLoc,translatey);
		gl.uniformMatrix3fv(rotationxLoc,false,rotatex);
		gl.uniformMatrix3fv(rotationyLoc,false,rotatey);

		gl.uniform1i(textureLoc, 0);

		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,indexBuffer);
		gl.drawElements(gl.TRIANGLES,indiceslength,gl.UNSIGNED_SHORT,0);
		gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,null);

		window.requestAnimationFrame(loop);
	}

window.requestAnimationFrame(loop);