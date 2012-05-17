<?php
	/**
	 ** frameset.php
	 */
	require_once("../v-load.php");
	require_once("../v-init.php");
	
	/** This part load LESS CSS for demo */
	v_LESS_load(v_section() . ".less");
	v_LESS_init();
?>

<html>
<head><title><?php echo __("Noname"); ?></title></head>
<body>
	<div class="frame1">
		<fieldset class="field1">
			<legend><?php echo __("Fieldset"); ?></legend>
			<label class="field1"><?php echo __("This is fieldset."); ?></label>
		</fieldset>
		<input type="input" value="<?php echo __("This is inputbox."); ?>" class="field1" />
		<input type="button" value="<?php echo __("This is button 1, try to click."); ?>" onclick="alert('<?php echo __("Hello, world."); ?>');" class="field1" />
		<input type="button" value="<?php echo __("This is button 2, try to click."); ?>" onclick="alert('<?php echo __("Hello, world."); ?>');" class="button1" />
		<p><?php echo __("The color here and fillcolor are not white. Moreover, its border is in dark gray."); ?></p>
		<div class="field1"><?php echo __("Embedded <div>"); ?></div>
	</div>
	<div class="frame2">
		<fieldset class="field2">
			<legend><?php echo __("Fieldset"); ?></legend>
			<label class=“field2”><?php echo __("This is fieldset."); ?></label>
		</fieldset>
		<input type="input" value="<?php echo __("This is inputbox."); ?>" class="field2" />
		<input type="button" value="<?php echo __("This is button 1, try to click."); ?>" onclick="alert('<?php echo __("Hello, world."); ?>');" class="field2" />
		<input type="button" value="<?php echo __("This is button 2, try to click."); ?>" onclick="alert('<?php echo __("Hello, world."); ?>');" class="button2" />
		<p><?php echo __("The elements inside are red; the background of frame is gray, and the border is green. There are scrollbars in the frame, fieldset and \"Embedded <div>\" below."); ?></p>
		<div class="field2"><?php echo __("Embedded <div>"); ?></div>
		<div class="field2"><?php echo __("Embedded <div>"); ?></div>
		<div class="field2"><?php echo __("Embedded <div>"); ?></div>
		<div class="field2"><?php echo __("Embedded <div>"); ?></div>
		<div class="field2"><?php echo __("Embedded <div>"); ?></div>
		<div class="field2"><?php echo __("Embedded <div>"); ?></div>
	</div>
</body>
</html>