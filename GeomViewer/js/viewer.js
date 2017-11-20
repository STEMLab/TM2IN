function findDestFrom(id, surface_list){
	//console.log("findDestFrom", id);
	let ret = [];
	for (var sf = 0 ; sf < surface_list.length ;sf++){
		var coords = surface_list[sf];
		for (var i = 0 ; i < coords.length - 1; i++){
			if (coords[i][3] == id) {
				ret.push(coords[i+1][3]);//return coords[i+1][3];
				//console.log(coords);
			}
		}
	}
	if (ret.length == 0) return -1;
	else {
		//console.log("multiple ret,", ret);
		return ret[0];
	}
}

function ccw(origin, coord1, coord2, index_list){
	var x1 = origin[index_list[0]],
	x2 = coord1[index_list[0]],
	x3 = coord2[index_list[0]],
	y1 = origin[index_list[1]],
	y2 = coord1[index_list[1]],
	y3 = coord2[index_list[1]];

	var temp = x1*y2+x2*y3+x3*y1;
    temp = temp - y1*x2-y2*x3-y3*x1;
    return temp;
}

function isExistInCoords(id, coords){
	for (var i = 0 ; i < coords.length ; i++){
		if (id == coords[i][3]) return i;
	}
	return -1;
}

function shareTwoVertices(big, small){
	var num = 0;
	for (var i = 0 ; i < small.length - 1 ; i++){
		for (var j = 0 ; j < big.length - 1 ; j++){
			if (small[i][3] == big[j][3]) {
				num++;
				if (num > 1) return true;
				break;
			}
		}
	}
	return false;
}

function EuclideanDistance(coord1, coord2){
	var ret = 0;
	for (var i = 0 ; i < 3 ; i++){
		ret += Math.pow(coord1[i] - coord2[i], 2);
	}
	ret = Math.sqrt(ret);
	return ret;
}

function makeBig(){
	console.log("makeBig");
	var floor = jsonfile.spaces[0].Surfaces[0].coord;
	var floor_zol = [];
	var zol_index_list = [];
	for (var i = 1 ; i < jsonfile.spaces[0].Surfaces.length ; i++){
		var sf_i = jsonfile.spaces[0].Surfaces[i];
		var normal = sf_i.normal;
		if (normal[2] * normal[2] > normal[1]* normal[1] + normal[0] * normal[0]){
			if (shareTwoVertices(floor, sf_i.coord)){
				console.log("add ", i);
				floor_zol.push(sf_i.coord);
				zol_index_list.push(i);
			}
		}
	}

	var new_surfaces = [];
	for (var i = 0 ; i < zol_index_list.length ; i++){
		new_surfaces.push(jsonfile.spaces[0].Surfaces[zol_index_list[i]]);
	}
	// jsonfile.spaces[0].Surfaces = new_surfaces;
	// refresh();
	// return;

	console.log("zol : ", floor_zol);

	var max_index = -1;
	for (var i = 0 ; i < floor.length - 1; i++){
		var is = false;
		var index_list = [0,1];
		for (var j = 0 ; j < floor.length - 1; j ++){
			if (j == i || j == i + 1) continue;
			if (ccw(floor[i], floor[i+1], floor[j], index_list) < 0) {
				is = true;
				break;
			}
		}
		if (!is) {
			max_index = i;
			break;
		}
	}

	console.log("max_index : ",max_index);
	if (max_index == -1) return;
	var big_coords = [];
	big_coords.push(floor[max_index]);

	var checked = new Array(floor.length);
	checked.fill(false);
	checked[max_index] = true;
	var loop_count = 0;
	var curr_i = max_index + 1;
	if (curr_i == floor.length - 1) curr_i = 0;
	while (!checked[curr_i]){
		if (loop_count ++ > floor.length){
			console.log("infinite");
			return;
		}
		//if (loop_count == 30) break;
		big_coords.push(floor[curr_i]);
		var vertex_id = floor[curr_i][3];

		console.log("curr_i : ", curr_i)
		console.log("vertex_id", vertex_id);

		var find_id = findDestFrom(vertex_id, floor_zol);
		console.log("find_id", find_id);
		if (find_id == -1){
			checked[curr_i] = true;
			curr_i++;
			if (curr_i == floor.length -1) curr_i = 0;
			console.log("next ", curr_i);
		}
		else{
			var next_i = curr_i + 1;
			if (next_i == floor.length -1) next_i = 0;
			var floor_coord = floor[next_i];

			var zol_index = isExistInCoords(find_id, floor);
			if (zol_index == -1){
				console.log("wrong", loop_count);
				checked[curr_i] = true;
				curr_i = next_i;
				continue;
			}
			if (checked[zol_index]){
				console.log("zol_index_checked so go floor", next_i);
				checked[curr_i] = true;
				curr_i = next_i;
				continue;
			}
			var zol_coord = floor[zol_index];

			var index_list = [0,1];
			var ccw_value = ccw(floor[curr_i], floor_coord, zol_coord, index_list);

			if (ccw_value >= 0){//floor
				console.log("go floor", next_i);
				checked[curr_i] = true;
				curr_i = next_i;
			}
			else if(ccw_value < 0){//zol
				console.log("go zol", zol_index);
				if (zol_index > curr_i){
					checked.fill(true, curr_i, zol_index - curr_i);
				}
				else{
					checked.fill(true, curr_i);
					checked.fill(true, 0, zol_index);
				}
				//console.log(checked);
				curr_i = zol_index;

			}
			else{
				console.log("equal");
				return;
			}
		}
	}
	big_coords.push(floor[max_index]);
	for (var i = zol_index_list.length - 1; i >=0 ; i--){
		console.log(zol_index_list[i]);
		jsonfile.spaces[0].Surfaces.splice(zol_index_list[i],1);
	}
	jsonfile.spaces[0].Surfaces[0].coord = big_coords;
	//new_surfaces.push(jsonfile.spaces[0].Surfaces[0]);
	//jsonfile.spaces[0].Surfaces = new_surfaces;
	refresh();
}

function match(id1, id2){
	var coord1 = jsonfile.spaces[0].Surfaces[id1].coord;
	var coord2 = jsonfile.spaces[0].Surfaces[id2].coord;
	//coord2.splice(coord2.length - 1, 1);
	console.log("length : ",coord1.length, coord2.length);
	var start_i, num = 0;
	var start_j = -1;

	var new_coord1 = [];
	var pre_j = -1;
	for (var i = 0 ; i < coord1.length -1 ; i++){

		for (var j = 0 ;  j < coord2.length ; j++){
			if (coord1[i][3] == coord2[j][3]){
				console.log(i, j);
				if (j + 1 != pre_j){
					console.log("There is a gap", j+1, pre_j);
				}
				pre_j = j;
				//console.log(coord1[i], coord2[j]);
				num++;
				break;
				// new_coord1.push(coord1[i]);
			}
		}
	}
	// new_coord1.push(new_coord1[0]);
	// jsonfile.spaces[0].Surfaces[id1].coord = new_coord1;
	console.log(num);
	return;

	/*
	if (num != coord1.length - 1){
		console.log("not matched whole");
		return;
	}
	for (var j = coord2.length - 2; j >= 0 ; j--){
		if (coord1[0][3] == coord2[j][3]){
			start_j = j;
			break;
		}
	}
	if (start_j == -1){
		console.log("WRONG");
		return;
	}

	var i = 0;
	var remove_j = [];
	for (var j = start_j; i < coord1.length -1 ;){
		if (coord2[j][3] == coord1[i][3]){
			console.log("i ", i);
			i++;
		}
		else{
			var miss_start_j = j;
			while (coord2[j][3] != coord1[i][3]){
				j--;
				if (j == -1) j = coord2.length - 1;
			}
			var mis_length;
			if (miss_start_j < j){
				mis_length = miss_start_j + (coord2.length - j);
			}
			else {
				mis_length = miss_start_j - j;
			}
			if (mis_length <= 2){
				coord2.splice(j + 1, mis_length);
				console.log("mis_length ", mis_length);
			}
			else{
				console.log("too much miss");
			}
			console.log("i ", i);
			i++;
		}

		coord2.splice(j, 1);
		console.log("remove ", j);
		j--;

		if (j == -1) j = coord2.length - 1;
	}
	coord2.push(coord2[0]);
	jsonfile.spaces[0].Surfaces.splice(id1, 1);
	*/
}
