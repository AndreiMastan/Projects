<?php
require 'includes/db.php';
session_start();

if (!isset($_SESSION['user_id'])) {
    header("Location: login.html");
    exit();
}

$user_id = $_SESSION['user_id'];

$orders = $conn->query("SELECT * FROM Orders WHERE UserID = $user_id ORDER BY OrderDate DESC");
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>My Orders</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
</head>
<body class="bg-light">
<div class="container mt-5">
    <h2 class="text-danger mb-4">My Orders</h2>

    <?php if ($orders->num_rows > 0): ?>
        <?php while ($order = $orders->fetch_assoc()): ?>
            <div class="card mb-4 shadow-sm">
                <div class="card-body">
                    <h5 class="card-title">🧾 Order ID: <?= $order['OrderID'] ?> <small class="text-muted float-end"><?= $order['OrderDate'] ?></small></h5>
                    <ul class="list-group list-group-flush mb-3">
                        <?php
                        $order_id = $order['OrderID'];
                        $items = $conn->query("SELECT p.Name, o.Quantity, (p.Price * o.Quantity) AS Subtotal
                                               FROM OrderItems o
                                               JOIN Products p ON o.ProductID = p.ProductID
                                               WHERE o.OrderID = $order_id");

                        $total = 0;
                        while ($item = $items->fetch_assoc()):
                            $total += $item['Subtotal'];
                        ?>
                            <li class="list-group-item d-flex justify-content-between align-items-center">
                                <?= $item['Name'] ?> - <?= $item['Quantity'] ?> pcs
                                <span class="badge bg-primary rounded-pill">$<?= $item['Subtotal'] ?></span>
                            </li>
                        <?php endwhile; ?>
                    </ul>
                    <h6 class="fw-bold text-end">Total: <span class="text-success">$<?= $total ?></span></h6>
                </div>
            </div>
        <?php endwhile; ?>
    <?php else: ?>
        <div class="alert alert-info">You haven't placed any orders yet.</div>
    <?php endif; ?>

    <div class="d-flex gap-2">
        <a href="home.php" class="btn btn-secondary">🛍️ Back to Store</a>
        <a href="logout.php" class="btn btn-outline-danger">🚪 Logout</a>
    </div>
</div>
</body>
</html>
