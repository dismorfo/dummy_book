#!/usr/bin/env node

var request = require('request')
  , js2xmlparser = require('js2xmlparser')
  , fs = require('fs')

function books_callback (error, response, body) {

    if (!error && response.statusCode == 200) {
    
        var books = JSON.parse(body)
        
        books.books.forEach( function(element, index, array) {

           var config = {
               'url' : 'http://dev-dl-pa.home.nyu.edu/books/books/' + element.identifier  + '/book.json?limit=' + element.sequences,
               'auth': {
                   'user': 'alpha-user',
                   'pass': 'dlts2010',
                   'sendImmediately': true
               }
           }

           var book_callback = function(error, response, body) {
           
               if (!error && response.statusCode == 200) {

                   var book = { node : JSON.parse(body) }
                   
                   fs.writeFile( require('path').dirname(require.main.filename) + '/xml/' + element.identifier + '.xml' , js2xmlparser('add', book), function (err) {

                       if (err) return console.log(err)
             
                   })

               }
           
           }
           
           request(config, book_callback)          
            
        })
    }
}

var books_config = {
    'url' : 'http://dev-dl-pa.home.nyu.edu/books/books.json?collection=94b17163-75c2-42c4-b258-754e9fc9e0ea&limit=1000',
    'auth': {
        'user': 'alpha-user',
        'pass': 'dlts2010',
        'sendImmediately': true
    }
}

request(books_config, books_callback)