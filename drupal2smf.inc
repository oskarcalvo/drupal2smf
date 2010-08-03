<?php
/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Connect to smfª\'s database.
 * @return
 *    Mysql link or FALSE.
 */
function smf_db_connect() {
  static $link = FALSE;
  if (!$link) {
    $dbhost = variable_get('drupal2smf_dbhost', NULL);
    $dbuser = variable_get('drupal2smf_dbuser', NULL);
    $dbpass = variable_get('drupal2smf_dbpass', NULL);
    $dbname = variable_get('drupal2smf_dbname', NULL);
    $smf_prefix = variable_get('drupal2smf_prefix', NULL);

    $link = mysql_connect($dbhost, $dbuser, $dbpass, TRUE, 2);
    if (!$link) {
      drupal_set_message(t('Unable to connect to database at «%host» with user «%user» and password «%pass».', array('%host' => $dbhost, '%user' => $dbuser, '%pass' => $dbpass)), 'error');
    }
    elseif (!mysql_select_db($dbname)) {
        drupal_set_message(t('Unable to select database :db.', array(':db' => $dbname)), 'error');
        $link = FALSE;
    }
  }

  return $link;
}

/**
 *
 * @param <type> $variable the tid to get the value of comment that response to one taxonomy of the fourm
 * @return <type>  an object with the count of the comments.
 *
 */
function drupal2smf_count_coments($tid){
    $query = db_query(" SELECT COUNT(*) as cantidad
                        FROM comments c INNER JOIN forum f ON(c.nid = f.nid)
                        where f.tid=%s;",$tid);
    $count = db_fetch_object($query);
    return $count->cantidad;    
}

/**
 *
 * @param <type> the $variable is the tid to get the count of the nodes forum from a taxonomy tid.
 * @return <type> an object with the count of the forum nodes of a taxonomy tid.
 *
 */

function drupal2smf_count_forum($tid){
    $query = db_query("SELECT COUNT(*) as cantidad FROM forum where tid=%s;",$tid);
    $count = db_fetch_object($query);
    return $count->cantidad;
}

/**
 *
 * @param <type> $variable
 * @return <type>
 *
 *
 */
function variable_vacia ($variable){
    $variable =($variable == '')? NULL : $variable ;
    return $variable;

}
/**
 *
 * @param <type> $nid we need the node id to recober it.
 * @return <type> en object with the options of cid or uid
 * if !cid, cid = 0
 * if !uid, uid = 0
 *
 *
 */

function drupal2smf_get_last_comment($nid){
  $query = db_fetch_object(db_query("select cid as cid , uid as uid from {comments} where nid=%d order by cid desc limit 0,1",$nid));
  return $query;
}
/**
 *
 * @param <type> $nid we need the node id to recober it.
 * @return <type> and object $query->replies with the number of nodes.
 *
 *
 */

function drupal2smf_get_count_comment($nid){
    $query = db_fetch_object(db_query("select count(cid) as cantidad from {comments} where nid=%d",$nid));
    return $query;
}



/**
 *
 * @param <type> $string a string of text to clean
 * @return <type> the string of text clean
 *
 *
 */

function drupal2msf_clean_string($string){
     $return2 = preg_replace(array(
		'~<p>~',
		'~</p>~',
		'~<a href=(.+?)>(.+?)</a>~',
		'~<strong>~',
		'~</strong>~',
		'~<em>~',
		'~</em>~',
		'~<code>~',
		'~</code>~',
		'~<(?:ul|ol)>~',
		'~</(?:ul|ol)>~',
		'~<li>~',
		'~</li>~',
		'~<pre>~',
		'~</pre>~',
		'~<dd>~',
		'~</dd>~',
		'~<dl>~',
		'~</dl>~',
		'~<dt>~',
		'~</dt>~',
		'~<cite>~',
		'~</cite>~',
		'~<\?php~',
		'~\?>~',
       '~\r\n~',

	),array(
		'',
		'',
		'[url=$1]$2[/url]',
		'[b]',
		'[/b]',
		'[i]',
		'[/i]',
		'[code]',
		'[/code]',
		'[list]',
		'[/list]',
		'[li]',
		'[/li]',
		'',
		'',
		'',
		'',
		'',
		'',
		'',
                '',
		'[quote]',
		'[/quote]',
		'[code]&lt;?php',
		'?&gt;[/code]',
        '<br>',
	),
	trim($string)
);
     $return = mysql_real_escape_string($return2);
     
     return $return;

}

/**
 *
 * @param <type> $tid is the tid of the categori with new id we want to smf.
 * @return <type> the new categori id.
 *
 *
 */


function drupal2smf_check_new_ID_cat($tid){
  $sql = db_fetch_object(db_query("SELECT * from {drupal2smf_categories_bridge} where taxonomy=%d",$tid));
  return $sql;

}

/**
 *
 * Create a temporal table to make the move from Drupal to smf.
 *
 *
 */

function drupal2smf_create_categories_bridge(){
  db_query("CREATE TABLE {drupal2smf_categories_bridge} (
            `bid` INT NOT NULL AUTO_INCREMENT PRIMARY KEY ,
            `taxonomy` INT NOT NULL ,
            `category` INT NOT NULL)
            ENGINE = MYISAM CHARACTER SET utf8 COLLATE utf8_spanish2_ci;
            ");
}

/**
 *
 *
 * Create a temporal tabl to move the the forum taxonomy to smf boards
 *
 *
 */

function drupal2smf_create_boards_table(){
  db_query("CREATE TABLE {drupal2smf_boards_bridge} (
           `bid` INT NOT NULL AUTO_INCREMENT PRIMARY KEY ,
           `tid` INT NOT NULL ,
           `ID_BOARD` INT NOT NULL)
           ENGINE = MYISAM CHARACTER SET utf8 COLLATE utf8_spanish2_ci;");
}

/**
 *
 * @param <type> the tid to get the new ID_BOARDS
 * @return <type> an object with the query, only the ID_BOARDS can be recorevered.
 *
 *
 */

function drupal2smf_check_new_ID_board($tid){
  $sql = db_fetch_object(db_query("SELECT * from {drupal2smf_boards_bridge} where tid=%d",$tid));
  return $sql;
}