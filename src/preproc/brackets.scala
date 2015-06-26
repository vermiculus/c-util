import util.parsing.combinator._

class Comp extends RegexParsers with PackratParsers {

  override def skipWhitespace = false

  lazy val bracefree: PackratParser[String] = """[^{}]+""".r ^^ {
    case a => a
  }

  lazy val matching: PackratParser[String] = (
    "{" ~ rep(bracefree | matching) ~ "}") ^^ {
      case a ~ b ~ c => a + b.mkString("") + c
    }
}

object Brackets extends Comp {
  def main(args: Array[String])= {
    println(parseAll(matching, "{ foo  {hello 3 } {}}").get)
  }
}
