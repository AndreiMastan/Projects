<?php
require 'includes/db.php';
session_start();

if (!isset($_SESSION['user_id'])) {
    header("Location: login.html");
    exit();
}

$result = $conn->query("SELECT * FROM Products");
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>VolleyShop - Home</title>
    <!-- Bootstrap 5 CDN -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    <!-- Bootstrap Icons -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.5/font/bootstrap-icons.css" rel="stylesheet">
</head>
<body class="bg-light">
<div class="container mt-4">
    <h1 class="text-danger">Welcome to VolleyShop</h1>

    <div class="mb-3">
        <a href='basket.php' class="btn btn-outline-primary me-2">
            <i class="bi bi-cart-fill me-1"></i> View Basket
        </a>
        <a href='myorders.php' class="btn btn-outline-success me-2">
            <i class="bi bi-box-seam me-1"></i> My Orders
        </a>
        <a href='wishlist.php' class="btn btn-outline-warning me-2">
            <i class="bi bi-heart-fill me-1"></i> Wishlist
        </a>
        <a href='logout.php' class="btn btn-outline-danger">
            <i class="bi bi-door-open-fill me-1"></i> Logout
        </a>
    </div>

    <?php if ($result->num_rows > 0): ?>
        <div class="row">
            <?php while ($row = $result->fetch_assoc()): ?>
                <div class="col-md-3 col-sm-6 mb-4">
                    <div class="card h-100 shadow-sm">
                        <div class="text-center" style="height: 180px; overflow: hidden;">
                            <img src="<?= htmlspecialchars($row['ImagePath']) ?>" class="img-fluid p-2" alt="<?= htmlspecialchars($row['Name']) ?>" style="max-height: 100%; object-fit: contain;">
                        </div>
                        <div class="card-body">
                            <h5 class="card-title"><?= htmlspecialchars($row['Name']) ?></h5>
                            <p class="card-text">
                                <strong>Category:</strong> <?= htmlspecialchars($row['Category']) ?><br>
                                <strong>Price:</strong> $<?= $row['Price'] ?>
                            </p>
                            <a href="product.php?id=<?= $row['ProductID'] ?>" class="btn btn-primary w-100">
                                View Product
                            </a>
                        </div>
                    </div>
                </div>
            <?php endwhile; ?>
        </div>
    <?php else: ?>
        <div class="alert alert-warning">No products available.</div>
    <?php endif; ?>

    <!-- About Us Section -->
    <?php
    $about_result = $conn->query("SELECT Message FROM AboutUs LIMIT 1");
    if ($about_result && $about_result->num_rows > 0):
        $about = $about_result->fetch_assoc();
    ?>
        <div class="card mt-5 shadow-sm">
            <div class="card-body">
                <h5 class="card-title text-muted">About Us</h5>
                <p class="card-text"><?= nl2br(htmlspecialchars($about['Message'])) ?></p>
            </div>
        </div>
    <?php endif; ?>
</div>
</body>
</html>
