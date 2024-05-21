import express from "express";
import { readFileSync, writeFileSync } from "fs";
import qrcode from "qrcode";

const app = express();
const PORT = 3000;
const PATH = "C:\\Users\\natey\\Desktop\\programming\\SIT\\102\\decant-tool-gen\\target\\keycodes.csv";
const URL_PREFIX = "http://10.141.63.141:3000/pallet?generate=false&keycode="

function readCSV(path) {
    return readFileSync(path, 'utf8');
}

function saveToCSV(csvString, path, keycodeStr) {
    if (csvString.length == 0 || path.length == 0 || keycodeStr.length == 0) return;
    const toWrite = csvString + "," + keycodeStr;
    writeFileSync(path, toWrite);
}

app.get('/', (req, res) => {
    const status = {
        "status": "running"
    };

    res.send(status);
})

app.get("/pallet", async (req, res) => {
    const queries = req.query;

    console.log(queries);

    if (!queries || !queries.keycode || !queries.generate) res.status(400).send("bad!");

    if (queries.generate == "true") {

        try {
            const qrCodeImage = await qrcode.toDataURL(URL_PREFIX + queries.keycode);
            res.status(200).send(`<img src="${qrCodeImage}" alt="QR Code"/>`);
        } catch (err) {
            res.status(500).send("An error occured!");
        }

    } else if (queries.generate == "false") {
        const csvString = readCSV(PATH);
        saveToCSV(csvString, PATH, queries.keycode);

        res.status(200).send(`Uploading keycode (${queries.keycode}) complete!`);
        return;
    } else {
        res.status(500).send(`An error occured.`);
    }
});

app.listen(PORT, () => {
    console.log(`listening on port ${PORT}`);
});