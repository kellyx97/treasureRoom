const express = require("express");
const app = express();
const path = require("path");
const bodyParser = require("body-parser");
const port = process.env.PORT || 3000;
const mongoose = require('./db.js');
const Thought = mongoose.model('Thought');

app.set("view engine", "ejs");
app.use(express.static(path.join(__dirname, 'public'))); // location of scripts and styles
app.use(bodyParser.urlencoded({ extended: false })); // needed to parse POST requests

// Renders the home page
app.get("/", function(req, res){
    res.render("index");
});

app.post("/api/create", function(req, res) {

    var date = new Date();


    console.log("POST request received");

    Thought.create({
        text: "this is a test",
        dateTime: date.getTime().toString()
    }, function(err, thought) {
        if (err) {
            console.error("Error creating new thought: " + err);
        } else {
            console.log("done");
        }
    });
});

app.listen(port, function(){
    console.log("Listening on port " + port);
});
