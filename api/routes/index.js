import express from "express";
import { readFile, readFileSync, writeFileSync } from "fs";
import qrcode from "qrcode";
import { fileURLToPath } from "url";
import path from "path";

// set up config
const __filename = fileURLToPath(import.meta.url);

const CONFIG_PATH = path.join(path.dirname(__filename), "..", "..", "config.json");
const config = JSON.parse(readFileSync(CONFIG_PATH));

const PORT = config.api.port || PORT;
const CSV_PATH = config.api.path_override.length > 0 ? config.api.path_override : path.join(path.dirname(__filename), "..", "..", "target", "keycodes.csv");
const IPV4_ADDRESS = process.env.IPV4_ADDRESS;
const URL_PREFIX = `http://${IPV4_ADDRESS}:${PORT}/pallet?generate=false&keycode=`

const app = express();

function readCSV(path) {
    return readFileSync(path, 'utf8');
}

function saveToCSV(csvString, path, keycodeStr) {
    if (csvString.length == 0 || path.length == 0 || keycodeStr.length == 0) return;
    const toWrite = csvString + "," + keycodeStr;
    writeFileSync(path, toWrite);
}

// default
app.get('/', (req, res) => {
    res.send({
        status: "running",
    });
})

app.get("/pallet", async (req, res) => {
    const queries = req.query;

    // not what we were looking for
    if (!queries || !queries.keycode) {
        res.status(400).send("An error occured!");
        return;
    }

    if (queries.keycode.length != 8) {
        res.status(400).send("An error occured!");
        return;
    }

    if (!queries.generate) queries.generate = 'false';

    if (queries.generate == "true") { // if they want a qrcode
        try {
            const qrCodeImage = await qrcode.toDataURL(URL_PREFIX + queries.keycode);
            res.status(200).send(`<img src="${qrCodeImage}" alt="QR Code"/>`);
        } catch (err) {
            res.status(500).send("An error occured!");
        }
    } else if (queries.generate == "false") { // if they have scanned a qr code
        saveToCSV(readCSV(CSV_PATH), CSV_PATH, queries.keycode);
        res.status(200).send(`Uploading keycode (${queries.keycode}) complete!`);
    } else { // they provided a value for generate that we werent expecting
        res.status(500).send(`An error occured.`);
    }
});

app.listen(PORT, () => {
    console.log(`listening on port ${PORT}`);
});