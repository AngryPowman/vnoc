<?php
	switch($_GET['section']){
		case "locations":
			file_put_contents("locations.csv", "Info,frame_pos,window_pos_1,window_pos_2\n");
			break;
		case "frameset":
			file_put_contents("frameset.csv", "Info,report\n");
			break;
		default:
			// 枚举终止;
	}
?>