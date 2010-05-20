<?
$basedir = $_SERVER['DOCUMENT_ROOT'] . "/uploads";
$fmt = $_SERVER['DOCUMENT_ROOT'] . "../cgi-bin/spritecutter/spritecutter -o%s -p%s %s";
$zip = "/usr/bin/zip -j %s %s";

if (isset($_FILES['file']['name'])) {
    extract($_FILES['file']);
    $tmpdirname = substr($tmp_name, 8, strlen($tmp_name));
    $path = $basedir . '/' . $tmpdirname . '/sprites/';

    mkdir($path, 0755, true);
    move_uploaded_file($tmp_name, $path . $name);

    $spritecutter = escapeshellcmd(sprintf($fmt, $path, "spr_", $path . $name));
    exec($spritecutter);

    $zip = sprintf($zip, $path . "../sprites.zip", $path . "*");
    exec($zip);
}
?>
<!DOCTYPE htm PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
  <title>Sprite cutter</title>
  <link rel="stylesheet" type="text/css" href="css/main.css" />
  <meta http-equiv="content-type" 
    content="text/html;charset=utf-8" />
  <meta http-equiv="Content-Style-Type" content="text/css" />
<script type="text/javascript">
$(function(){
});
</script>
<style type="text/css">
.hidden {display:none;}
.orange {color:#E88C4A;}
div#output {border-top:1px dashed black;padding:20px;}
div#upload-sprite {background:#eeeeee;padding:20px;margin-bottom:20px;}
.redoutline {border:1px dashed red;}
.row img {margin:10px;float:left;border:1px dashed red;padding:2px;}

</style>
</head>
<body>
<div id="container">
<h1 class="orange">Sprite Cutter</h1>    
<div id="upload-sprite"> 
<h2>Upload Sprite Sheet</h2>
  <form action="<?$_SERVER['REQUEST_URI'];?>" method="post" enctype="multipart/form-data">
    <div>
      <div>
        <input id="file" class="button" name="file" type="file">
      </div>
      <div>
        <input id="cut-sprite-button" class="button" value="Cut Sprite" type="submit">
      </div>
    </div>
  </form>
</div>
<?if(isset($_FILES['file']['name'])) {?>
  <div id="output">
    <h2>Output</h2>
    <a class="orange" href="uploads/<?=$tmpdirname;?>/sprites.zip">Download .zip</a>
  </div>
  <div>
    <img src="uploads/<?=$tmpdirname;?>/sprites/spr_label.png" />
  </div>
<?}?>
</div>
</body>
</html>
