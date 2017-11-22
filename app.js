const express = require("express");
const app = express();
const path = require("path");
const bodyParser = require("body-parser");
const port = process.env.PORT || 3000;

app.set("view engine", "ejs");
app.use(express.static(path.join(__dirname, 'public'))); // location of scripts and styles
app.use(bodyParser.urlencoded({ extended: false })); // needed to parse POST requests

// Renders the home page
app.get("/", function(req, res){
    res.render("index");
});

app.get("/", function(req, res) {

});

app.listen(port, function(){
    console.log("Listening on port " + port);
});

// Database
const mongoose = require('mongoose');
var testUser = "test";
var testPass = "test";

var thoughtSchema = mongoose.Schema({
    text: {type: String, required: true},
    dateTime: {type: String, required: true}
});

mongoose.model('Thought', thoughtSchema, 'thoughts');
