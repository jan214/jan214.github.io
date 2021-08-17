function OBJLoader(text){
	var lines = text.split("\n");
	var vertices = new Array();
	var normals = new Array();
	var textures = new Array();
	var indices = new Array();
	var normalsIndices = new Array();
	var texturesIndices = new Array();
	var line;
	var indicesreal = new Array();
	var verticesreal = new Array();
	var normalsreal = new Array();
	var texturesreal = new Array();
	var colors = new Array();
	var firsti, secondi, thirdi, fourthi;
	var firstt, secondt, thirdt, fourtht;
	var firstn, secondn, thirdn, fourthn;

	this.getIndices = function(){
		return indicesreal;
	}

	this.getVertices = function(){
		return verticesreal;
	}

	this.getNormals = function(){
		return normalsreal;
	}

	this.getTextures = function(){
		return texturesreal;
	}

	this.getColors = function(){
		return colors;
	}

	for(var i = 0; i < lines.length; i++){
		if(lines[i].startsWith("v ")){
			vertices.push(lines[i].split(" ")[1]);
			vertices.push(lines[i].split(" ")[2]);
			vertices.push(lines[i].split(" ")[3]);
		}else if(lines[i].startsWith("vn ")){
			normals.push(lines[i].split(" ")[1]);
			normals.push(lines[i].split(" ")[2]);
			normals.push(lines[i].split(" ")[3]);
		}else if(lines[i].startsWith("vt ")){
			textures.push(lines[i].split(" ")[1]);
			textures.push(lines[i].split(" ")[2]);
		}else if(lines[i].startsWith("f ")){
			line = lines[i].split("f ")[1];
			if(line.split(" ").length == 3){
				indices.push(line.substring(0,line.indexOf("/"))-1);
				line = line.substring(line.indexOf("/")+1,line.length);

				if(line.substring(0,line.indexOf("/")).length == 0){
					texturesIndices.push(0);
				}else{
					texturesIndices.push(line.substring(0,line.indexOf("/"))-1);
				}
				line = line.substring(line.indexOf("/")+1,line.length);

				normalsIndices.push(line.substring(0,line.indexOf(" "))-1);
				line = line.substring(line.indexOf(" "),line.length);

				indices.push(line.substring(0,line.indexOf("/"))-1);
				line = line.substring(line.indexOf("/")+1,line.length);

				if(line.substring(0,line.indexOf("/")).length == 0){
					texturesIndices.push(0);
				}else{
					texturesIndices.push(line.substring(0,line.indexOf("/"))-1);
				}
				line = line.substring(line.indexOf("/")+1,line.length);

				normalsIndices.push(line.substring(0,line.indexOf(" "))-1);
				line = line.substring(line.indexOf(" "),line.length);

				indices.push(line.substring(0,line.indexOf("/"))-1);
				line = line.substring(line.indexOf("/")+1,line.length);

				if(line.substring(0,line.indexOf("/")).length == 0){
					texturesIndices.push(0);
				}else{
					texturesIndices.push(line.substring(0,line.indexOf("/"))-1);
				}
				line = line.substring(line.indexOf("/")+1,line.length);

				normalsIndices.push(line);
			}else if(line.split(" ").length == 4){
				firsti = line.substring(0,line.indexOf("/"))-1;
				line = line.substring(line.indexOf("/")+1,line.length);

				if(line.substring(0,line.indexOf("/")).length == 0){
					firstt = 0;
				}else{
					firstt = line.substring(0,line.indexOf("/"))-1;
				}

				line = line.substring(line.indexOf("/")+1,line.length);

				firstn = line.substring(0,line.indexOf(" "))-1;
				line = line.substring(line.indexOf(" ")+1,line.length);


				secondi = line.substring(0,line.indexOf("/"))-1;
				line = line.substring(line.indexOf("/")+1,line.length);


				if(line.substring(0,line.indexOf("/")).length == 0){
					secondt = 0;
				}else{
					secondt = line.substring(0,line.indexOf("/"))-1;
				}
				line = line.substring(line.indexOf("/")+1,line.length);

				secondn = line.substring(0,line.indexOf(" "))-1;
				line = line.substring(line.indexOf(" ")+1,line.length);

				thirdi = line.substring(0,line.indexOf("/"))-1;
				line = line.substring(line.indexOf("/")+1,line.length);

				if(line.substring(0,line.indexOf("/")).length == 0){
					thirdt = 0;
				}else{
					thirdt = line.substring(0,line.indexOf("/"))-1;
				}
				line = line.substring(line.indexOf("/")+1,line.length);

				thirdn = line.substring(0,line.indexOf(" "))-1;
				line = line.substring(line.indexOf(" ")+1,line.length);

				fourthi = line.substring(0,line.indexOf("/"))-1;
				line = line.substring(line.indexOf("/")+1,line.length);

				if(line.substring(0,line.indexOf("/")).length == 0){
					fourtht = 0;
				}else{
					fourtht = line.substring(0,line.indexOf("/"))-1;
				}
				line = line.substring(line.indexOf("/")+1,line.length);

				fourthn = line-1;

				indices.push(firsti);
				indices.push(secondi);
				indices.push(thirdi);
				indices.push(firsti);
				indices.push(thirdi);
				indices.push(fourthi);

				normalsIndices.push(firstn);
				normalsIndices.push(secondn);
				normalsIndices.push(thirdn);
				normalsIndices.push(firstn);
				normalsIndices.push(thirdn);
				normalsIndices.push(fourthn);

				texturesIndices.push(firstt);
				texturesIndices.push(secondt);
				texturesIndices.push(thirdt);
				texturesIndices.push(firstt);
				texturesIndices.push(thirdt);
				texturesIndices.push(fourtht);
			}



			/*if(line.indexOf(" ") == -1){
				normalsIndices.push(line.substring(0,line.length)-1);
				line = line.substring(line.indexOf(" "),line.length);
			}else{
				normalsIndices.push(line.substring(0,line.indexOf(" "))-1);
				line = line.substring(line.indexOf(" ")+1,line.length);
				indices.push(line.substring(0,line.indexOf("/"))-1);
				line = line.substring(line.indexOf("/")+1,line.length);
				texturesIndices.push(line.substring(0,line.indexOf("/"))-1);
				normalsIndices.push(line.substring(line.indexOf("/")+1,line.length)-1);
			}*/
		}
	}

	/*
	console.log(vertices);
	console.log(normals);
	console.log(textures);
	console.log(indices);
	console.log(normalsIndices);
	console.log(texturesIndices);
	*/

	for(var i = 0; i < indices.length; i++){
		indicesreal.push(i);
		verticesreal.push(vertices[indices[i]*3]);
		verticesreal.push(vertices[indices[i]*3+1]);
		verticesreal.push(vertices[indices[i]*3+2]);
		normalsreal.push(normals[normalsIndices[i]*3]);
		normalsreal.push(normals[normalsIndices[i]*3+1]);
		normalsreal.push(normals[normalsIndices[i]*3+2]);
		if(textures.length != 0){
			texturesreal.push(textures[texturesIndices[i]*2]);
			texturesreal.push(textures[texturesIndices[i]*2+1]);
		}else{
			texturesreal.push(0.0);
			texturesreal.push(0.0);
		}
		colors.push(0.0);
		colors.push(0.0);
		colors.push(1.0);
	}

	/*
	console.log(indicesreal.length);
	console.log(verticesreal);
	console.log(normalsreal.length);
	console.log(texturesreal.length);
	*/
	
}