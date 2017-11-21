var express = require("express");
var app = express();
var path = require("path");
var bodyParser = require("body-parser");
var port = process.env.PORT || 3000;

app.set("view engine", "ejs");
app.use(express.static(path.join(__dirname, 'public'))); // location of scripts and styles
app.use(bodyParser.urlencoded({ extended: false })); // needed to parse POST requests

// Renders the home page
app.get("/", function(req, res){
    res.render("index");
});

app.listen(port, function(){
    console.log("Listening on port " + port);
});
