class Plane{
	vertices = new Float32Array([5,5,0,5,-5,0,-5,5,0, -5,5,0,5,-5,0,-5,-5,0]);
	uvs = new Float32Array([1,0,1,1,0,0, 0,0,1,1,0,1]);
	normals = new Float32Array([0,0,-1,0,0,-1,0,0,-1, 0,0,-1,0,0,-1,0,0,-1]);
	tangents = new Float32Array([1,0,0,1,0,0,1,0,0, 1,0,0,1,0,0,1,0,0]);

	vbuffer;
	nbuffer;
	tbuffer;
	tanbuffer;

	vpositionLoc;
	normalLoc;
	uvLoc;

	texture;
	normalmap;
	heightmap;

	atexture;
	texturesampler;
	anormalmap;
	normalsampler;
	aheightmap;
	heightsampler;

	constructor(shaderProgram){
		console.log(this.vertices.length+"/"+this.uvs.length+"/"+this.normals.length+"/"+this.tangents.length);
		this.vbuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, this.vbuffer);
		gl.bufferData(gl.ARRAY_BUFFER,this.vertices,gl.STATIC_DRAW);

		this.vpositionLoc = gl.getAttribLocation(shaderProgram,"position");
		gl.vertexAttribPointer(this.vpositionLoc,3,gl.FLOAT,false,0,0);
		gl.enableVertexAttribArray(this.vpositionLoc);

		this.tbuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,this.tbuffer);
		gl.bufferData(gl.ARRAY_BUFFER,this.uvs,gl.STATIC_DRAW);

		this.uvLoc = gl.getAttribLocation(shaderProgram,"uv");
		gl.vertexAttribPointer(this.uvLoc,2,gl.FLOAT,false,0,0);
		gl.enableVertexAttribArray(this.uvLoc);

		this.nbuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,this.nbuffer);
		gl.bufferData(gl.ARRAY_BUFFER,this.normals,gl.STATIC_DRAW);

		this.normalLoc = gl.getAttribLocation(shaderProgram,"normal");
		gl.vertexAttribPointer(this.normalLoc,3,gl.FLOAT,false,0,0);
		gl.enableVertexAttribArray(this.normalLoc);

		this.tanbuffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER,this.tanbuffer);
		gl.bufferData(gl.ARRAY_BUFFER,this.tangents,gl.STATIC_DRAW);

		this.tanLoc = gl.getAttribLocation(shaderProgram,"tangent");
		gl.vertexAttribPointer(this.tanLoc,3,gl.FLOAT,false,0,0);
		gl.enableVertexAttribArray(this.tanLoc);

		this.atexture = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D,this.atexture);

		this.texture = new Image();
		this.texture.src = "texture.png";

		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
		gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);

		this.texture.addEventListener("load",() => {
			gl.bindTexture(gl.TEXTURE_2D,this.atexture);
			gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,this.texture);
		});

		this.texturesampler = gl.getUniformLocation(shaderProgram, "texture");

		this.anormalmap = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D,this.anormalmap);

		this.normalmap = new Image();
		this.normalmap.src = "normalmap.png";

		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
		gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);

		this.normalmap.addEventListener("load",() => {
			gl.bindTexture(gl.TEXTURE_2D,this.anormalmap);
			gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,this.normalmap);
		});

		this.normalsampler = gl.getUniformLocation(shaderProgram, "normalmap");

		this.aheightmap = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D,this.aheightmap);

		this.heightmap = new Image();
		this.heightmap.src = "heightmap.png";

		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
		gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);

		this.heightmap.addEventListener("load",() => {
			gl.bindTexture(gl.TEXTURE_2D,this.aheightmap);
			gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,gl.RGBA,gl.UNSIGNED_BYTE,this.heightmap);
		});

		this.heightsampler = gl.getUniformLocation(shaderProgram, "heightmap");
	}

	update(){
	}

	draw(){
		gl.bindBuffer(gl.ARRAY_BUFFER,this.vbuffer);
		gl.vertexAttribPointer(this.vpositionLoc,3,gl.FLOAT,false,0,0);
		gl.enableVertexAttribArray(this.vpositionLoc);

		gl.bindBuffer(gl.ARRAY_BUFFER,this.tbuffer);
		gl.vertexAttribPointer(this.uvLoc,2,gl.FLOAT,false,0,0);
		gl.enableVertexAttribArray(this.uvLoc);

		gl.bindBuffer(gl.ARRAY_BUFFER,this.nbuffer);
		gl.vertexAttribPointer(this.normalLoc,3,gl.FLOAT,false,0,0);
		gl.enableVertexAttribArray(this.normalLoc);

		gl.bindBuffer(gl.ARRAY_BUFFER,this.tanbuffer);
		gl.vertexAttribPointer(this.tanLoc,3,gl.FLOAT,false,0,0);
		gl.enableVertexAttribArray(this.tanLoc);

		gl.uniform1i(this.texturesampler,0);
		gl.activeTexture(gl.TEXTURE0);
		gl.bindTexture(gl.TEXTURE_2D,this.atexture);

		gl.uniform1i(this.normalsampler,1);
		gl.activeTexture(gl.TEXTURE1);
		gl.bindTexture(gl.TEXTURE_2D,this.anormalmap);

		gl.uniform1i(this.heightsampler,2);
		gl.activeTexture(gl.TEXTURE2);
		gl.bindTexture(gl.TEXTURE_2D,this.aheightmap);

		gl.enable(gl.CULL_FACE);
		gl.cullFace(gl.FRONT);

		gl.drawArrays(gl.TRIANGLES,0,6);
	}
}