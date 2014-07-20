#!/usr/bin/env node

var request = require('request')
  , js2xmlparser = require('js2xmlparser')
  , fs = require('fs')

function books_callback (error, response, body) {

    if (!error && response.statusCode == 200) {
    
        var books = JSON.parse(body)
        
        books.books.forEach( function(element, index, array) {

           var config = {
               'url' : element.uri + '/metadata.json?limit=' + element.sequences,
               'auth': {
                   'user': 'alpha-user',
                   'pass': 'dlts2010',
                   'sendImmediately': true
               }
           }
           
           var book_callback = function(error, response, body) {
           
               if (!error && response.statusCode == 200) {

                   var book = { node : JSON.parse(body) }
                   
                   fs.writeFile( require('path').dirname(require.main.filename) + '/xml/awdl/' + element.identifier + '.xml' , js2xmlparser('add', book), function (err) {

                       if (err) return console.log(err)
             
                   })

               }
           
           }
           
           request(config, book_callback)          
            
        })
    }
}

var books_config = {
    'url' : 'http://localhost:8000/awdl6/datasources/books.json',
    'auth': {
        'user': 'alpha-user',
        'pass': 'dlts2010',
        'sendImmediately': true
    }
}

request(books_config, books_callback)