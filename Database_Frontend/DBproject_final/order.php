<?php
require 'includes/db.php';
session_start();

if (!isset($_SESSION['user_id'])) {
    header("Location: login.html");
    exit();
}

$user_id = $_SESSION['user_id'];
$result = $conn->query("SELECT p.Name, b.Quantity, (p.Price * b.Quantity) AS Subtotal
                        FROM BasketItems b
                        JOIN Basket bk ON b.BasketID = bk.BasketID
                        JOIN Products p ON b.ProductID = p.ProductID
                        WHERE bk.UserID = $user_id");

$total = 0;
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Confirm Order</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
</head>
<body class="bg-light">
<div class="container mt-5">
    <h2 class="text-danger mb-4">Confirm Your Order</h2>

    <div class="card shadow-sm">
        <div class="card-body">
            <?php if ($result->num_rows > 0): ?>
                <ul class="list-group mb-3">
                    <?php while ($row = $result->fetch_assoc()): ?>
                        <li class="list-group-item d-flex justify-content-between align-items-center">
                            <?= $row['Name'] ?> - <?= $row['Quantity'] ?> pcs
                            <span class="badge bg-primary rounded-pill">$<?= $row['Subtotal'] ?></span>
                        </li>
                        <?php $total += $row['Subtotal']; ?>
                    <?php endwhile; ?>
                </ul>

                <h5>Total: <span class="text-success">$<?= $total ?></span></h5>

                <form method="post" action="place_order.php" class="mt-3">
                    <button type="submit" class="btn btn-success">✅ Place Order</button>
                </form>

                <a href="basket.php" class="btn btn-outline-secondary mt-2">← Back to Basket</a>

            <?php else: ?>
                <div class="alert alert-info">Your basket is empty.</div>
                <a href="home.php" class="btn btn-secondary">Go to Store</a>
            <?php endif; ?>
        </div>
    </div>
</div>
</body>
</html>
