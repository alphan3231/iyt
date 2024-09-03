<?php

require 'vendor/autoload.php';  // Composer ile kurduğunuz kütüphaneyi dahil edin

use PhpOffice\PhpSpreadsheet\Spreadsheet;
use PhpOffice\PhpSpreadsheet\Writer\Xlsx;

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Gelen verileri alın
    $personID = $_POST['personID'] ?? "N/A";
    $areaID = $_POST['areaID'] ?? "N/A";
    $bpm = $_POST['bpm'] ?? "N/A";
    $latitude = $_POST['latitude'] ?? "N/A";
    $longitude = $_POST['longitude'] ?? "N/A";
    $altitude = $_POST['altitude'] ?? "N/A";

    // Excel dosyası oluştur
    $spreadsheet = new Spreadsheet();
    $sheet = $spreadsheet->getActiveSheet();

    // Başlıkları yaz
    $sheet->setCellValue('A1', 'personID');
    $sheet->setCellValue('B1', 'areaID');
    $sheet->setCellValue('C1', 'bpm');
    $sheet->setCellValue('D1', 'latitude');
    $sheet->setCellValue('E1', 'longitude');
    $sheet->setCellValue('F1', 'altitude');

    // Gelen verileri yaz
    $sheet->setCellValue('A2', $personID);
    $sheet->setCellValue('B2', $areaID);
    $sheet->setCellValue('C2', $bpm);
    $sheet->setCellValue('D2', $latitude);
    $sheet->setCellValue('E2', $longitude);
    $sheet->setCellValue('F2', $altitude);

    // Dosya adı ve yolu
    $fileName = 'data.xlsx';
    $writer = new Xlsx($spreadsheet);
    $writer->save($fileName);

    echo "Data received and saved to Excel.";

} else {
    echo "Please send a POST request.";
}
?>
