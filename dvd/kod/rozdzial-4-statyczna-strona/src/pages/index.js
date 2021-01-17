import React from "react"
import { Link } from "gatsby"
import Layout from "../components/layout"
import SEO from "../components/seo"
import MeImgSrc from "../images/ja.jpg"

const IndexPage = () => (
  <Layout>
    <SEO title="Home" />
    <div>
      <h2>Artur Kasperek - Programista oraz Student Politechniki Śląśkiej</h2>
      <img src={MeImgSrc}/>
    </div>
    <div>
      <Link to="/detale">Więcej o mnie</Link> <br />
    </div>
  </Layout>
)

export default IndexPage
