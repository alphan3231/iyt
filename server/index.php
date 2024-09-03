<?php

require 'vendor/autoload.php';  // Composer ile kurduğunuz PhpSpreadsheet kütüphanesini dahil edin

use PhpOffice\PhpSpreadsheet\IOFactory;

$fileName = 'data.xlsx';  // Excel dosyası adı

// Verileri oku
$data = [];
if (file_exists($fileName)) {
    $spreadsheet = IOFactory::load($fileName);
    $sheet = $spreadsheet->getActiveSheet();
    $data = $sheet->toArray();
} else {
    echo "Excel dosyası bulunamadı.";
    exit;
}

?>

<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Veri Görüntüleme</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            color: #333;
            padding: 20px;
        }
        h1 {
            text-align: center;
            color: #4CAF50;
        }
        table {
            margin: 0 auto;
            border-collapse: collapse;
            width: 80%;
            background-color: #fff;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
        }
        th, td {
            padding: 12px;
            border: 1px solid #ddd;
            text-align: center;
        }
        th {
            background-color: #4CAF50;
            color: white;
        }
        tr:nth-child(even) {
            background-color: #f2f2f2;
        }
        .no-data {
            text-align: center;
            margin-top: 20px;
            font-size: 18px;
        }
    </style>
</head>
<body>

<h1>Gönderilen Veriler</h1>

<?php if (!empty($data) && count($data) > 1): ?>
    <table>
        <thead>
            <tr>
                <?php foreach ($data[0] as $header): ?>
                    <th><?php echo htmlspecialchars($header); ?></th>
                <?php endforeach; ?>
            </tr>
        </thead>
        <tbody>
            <?php for ($i = 1; $i < count($data); $i++): ?>
                <tr>
                    <?php foreach ($data[$i] as $cell): ?>
                        <td><?php echo htmlspecialchars($cell); ?></td>
                    <?php endforeach; ?>
                </tr>
            <?php endfor; ?>
        </tbody>
    </table>
<?php else: ?>
    <div class="no-data">Görüntülenecek veri yok.</div>
<?php endif; ?>

</body>
</html>
