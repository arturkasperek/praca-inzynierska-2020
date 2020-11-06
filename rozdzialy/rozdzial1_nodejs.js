const express = require('express')
const app = express()
const port = 3000
const users = [
    {
        name: 'Jan',
        email: 'jan@gmail.com',
    }, 
    {
        name: 'Kasia',
        email: 'kasia@gmail.com',
    }
]

app.get('/', (req, res) => {
    res.send(users)
})

app.listen(port, () => {
    console.log(`Users service listening at http://localhost:${port}`)
})