import React from "react"
import { Link } from "gatsby"

import Layout from "../components/layout"
import SEO from "../components/seo"

const SecondPage = () => (
  <Layout>
    <SEO title="Detale" />
    <h2>Doświadczenie</h2>
    <ul>
      <li>JavaScript</li>
      <li>HTML</li>
      <li>CSS</li>
      <li>C++</li>
      <li>NodeJS</li>
    </ul>
    <h2>Szkoły</h2>
    <ul>
      <li>Politechnika Śląska</li>
      <li>I Liceum Ogólnokształcącego im. Stefana Żeromskiego w Zawierciu</li>
      <li>Gimnazjum nr 1 Zawierciu</li>
    </ul>
    <Link to="/">Powrót do strony domowej</Link>
  </Layout>
)

export default SecondPage
