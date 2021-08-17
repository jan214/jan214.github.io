const canvas = document.getElementById("canvas");
const gl = canvas.getContext('webgl');

const vSource = "attribute vec3 position;"+
				"attribute vec3 normal;"+
				"attribute vec2 uv;"+
				"attribute vec3 tangent;"+
				"uniform vec3 translate;"+
				"uniform mat3 rotatex;"+
				"uniform mat3 rotatey;"+
				"uniform mat3 rotatez;"+
				"uniform vec3 lightpos;"+
				"uniform vec3 lightcolor;"+
				"uniform vec3 camerapos;"+
				"vec3 aposition;"+
				"mat3 rotation;"+
				"varying vec3 apos;"+
				"varying vec3 anormal;"+
				"varying vec3 alightpos;"+
				"varying vec3 alightcolor;"+
				"varying vec3 acamerapos;"+
				"varying vec2 auv;"+
				"varying vec3 atangent;"+
				"varying vec3 abitangent;"+
				"vec3 T;"+
				"vec3 B;"+
				"vec3 N;"+
				"mat3 TBN;"+
				"void main(){"+
				"rotation = rotatey * rotatex;"+
				"aposition = position * rotation + translate;"+
				"apos = aposition;"+
				"alightpos = lightpos + translate;"+
				"auv = uv;"+
				"alightcolor = lightcolor;"+
				"acamerapos = camerapos;"+
				"abitangent = cross(normal,tangent);"+
				//setup TBN matrix
				"T = normalize(tangent * rotation);"+
				"B = normalize(abitangent * rotation);"+
				"N = normalize(normal * rotation);"+
				"TBN[0] = T;"+
				"TBN[1] = B;"+
				"TBN[2] = N;"+
				//transpose TBN Matrix
				"TBN[0] = vec3(TBN[0][0],TBN[1][0],TBN[2][0]);"+
				"TBN[1] = vec3(TBN[0][1],TBN[1][1],TBN[2][1]);"+
				"TBN[2] = vec3(TBN[0][2],TBN[1][2],TBN[2][2]);"+
				//move apos alightpos and acamerpos into tangent space
				"apos = TBN * apos;"+
				"alightpos = TBN * alightpos;"+
				"acamerapos = TBN * acamerapos;"+
				"gl_Position = vec4(aposition.x,aposition.y,aposition.z,(1.0+aposition.z+translate.z));"+
				"}";

var vertShader = gl.createShader(gl.VERTEX_SHADER);
gl.shaderSource(vertShader,vSource);
gl.compileShader(vertShader);

const fSource = "precision mediump float;"+
				"varying vec3 apos;"+
				"varying vec3 anormal;"+
				"varying vec3 alightpos;"+
				"varying vec3 alightcolor;"+
				"varying vec3 acamerapos;"+
				"varying vec2 auv;"+
				"varying vec3 atangent;"+
				"varying vec3 abitangent;"+
				"uniform sampler2D texture;"+
				"uniform sampler2D normalmap;"+
				"uniform sampler2D heightmap;"+
				"vec3 unpackednormal;"+
				"vec2 parralaxpoint;"+
				"vec3 lightdir;"+
				"vec3 viewdir;"+
				"float diffuse;"+
				"vec3 reflectdir;"+
				"float specular;"+
				"vec3 color;"+
				"float scale;"+
				"void main(){"+
				"viewdir = normalize(acamerapos - apos);"+
				//get height value from heightmap
				"scale = texture2D(heightmap,auv).r;"+
				//calculate coordinate on texture
				"parralaxpoint = auv - ((viewdir.xy/viewdir.z) * (scale * 0.1));"+
				//get normal from normalmap
				"unpackednormal = normalize(texture2D(normalmap,parralaxpoint).xyz * 2.0 - 1.0);"+
				"lightdir = normalize(apos-alightpos);"+
				"diffuse = max(dot(lightdir,unpackednormal),0.1);"+
				"reflectdir = normalize(reflect(-lightdir,unpackednormal));"+
				"specular = dot(viewdir,reflectdir);"+
				"specular = max(pow(specular,64.0),0.1);"+
				"color = texture2D(texture,parralaxpoint).xyz*diffuse+alightcolor*specular;"+
				"gl_FragColor = vec4(color,1.0);"+
				"}";

var fragShader = gl.createShader(gl.FRAGMENT_SHADER);
gl.shaderSource(fragShader,fSource);
gl.compileShader(fragShader);

if (!gl.getShaderParameter(vertShader, gl.COMPILE_STATUS)) {
    alert(gl.getShaderInfoLog(vertShader));
}

var shaderProgram = gl.createProgram();
gl.attachShader(shaderProgram,vertShader);
gl.attachShader(shaderProgram,fragShader);
gl.linkProgram(shaderProgram);
gl.useProgram(shaderProgram);

const plane = new Plane(shaderProgram);

//translation and rotation uniforms
var translatev = new Float32Array([0.0,0.0,5.0]);

var rotationx = new Float32Array([1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0]);
var rotationy = new Float32Array([1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0]);
var rotationz = new Float32Array([1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0]);
var px = 0.0;
var tx = 0.0;
var lx = 0.0;
var rx = 0.0;
var ry = 0.0;

//object uniforms
var lightpos = [0.0,0.0,1.0];
var lightcolor = [1.0,1.0,1.0];
var camerapos = [0.0,0.0,0.0];

var deltaTime = 0.0;
var now = Date.now();

var mousedown = false;
var mousepos = new Array([0.0,0.0]);

//setup uniforms
var translateU = gl.getUniformLocation(shaderProgram,"translate");
gl.uniform3fv(translateU,translatev);

var rotatexU = gl.getUniformLocation(shaderProgram,"rotatex");
gl.uniformMatrix3fv(rotatexU,false,rotationx);

var rotateyU = gl.getUniformLocation(shaderProgram,"rotatey");
gl.uniformMatrix3fv(rotateyU,false,rotationy);

var rotatezU = gl.getUniformLocation(shaderProgram,"rotatez");
gl.uniformMatrix3fv(rotatezU,false,rotationz);

var lightposU = gl.getUniformLocation(shaderProgram,"lightpos");
gl.uniform3fv(lightposU,lightpos);

var lightcolorU = gl.getUniformLocation(shaderProgram,"lightcolor");
gl.uniform3fv(lightcolorU,lightcolor);

var cameraposU = gl.getUniformLocation(shaderProgram,"camerapos");
gl.uniform3fv(cameraposU,camerapos);

canvas.addEventListener('mousedown', event => {
	mousedown = true;
	mousepos[0] = event.pageX;
	mousepos[1] = event.pageY;
});

canvas.addEventListener('mousemove', event => {
	if(mousedown){
		ry += (event.pageX - mousepos[0])*deltaTime;

		mousepos[0] = event.pageX;

		rotationy[0] = Math.cos(ry);
		rotationy[2] = Math.sin(ry);
		rotationy[6] = -Math.sin(ry);
		rotationy[8] = Math.cos(ry);
	}
});

canvas.addEventListener('mouseup', event =>{
	mousedown = false;
});

function loop(){
	deltaTime = (Date.now() - now)*0.001;
	now = Date.now();

	gl.clearColor(0.0,0.0,0.0,1.0);
	gl.clear(gl.COLOR_BUFFER_BIT);

	gl.uniform3fv(translateU,translatev);
	gl.uniformMatrix3fv(rotatexU,false,rotationx);
	gl.uniformMatrix3fv(rotateyU,false,rotationy);
	gl.uniformMatrix3fv(rotatezU,false,rotationz);

	plane.draw();

	window.requestAnimationFrame(loop);
}

window.requestAnimationFrame(loop);