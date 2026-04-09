<?php
require 'includes/db.php';
session_start();
if (!isset($_SESSION['user_id'])) {
    header("Location: login.html");
    exit();
}

$item_id = $_POST['item_id'];
$conn->query("DELETE FROM BasketItems WHERE BasketItemID = $item_id");
header("Location: basket.php");
?>