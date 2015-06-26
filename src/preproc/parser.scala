import util.parsing.combinator._

abstract class Param
case class Typed(ty: String, nm: String) extends Param
case class Method(ty: Typed, args: List[Typed], src: String) extends Param

case class Class(name: String, pubs: List[Param], priv: List[Param])

class Comp extends RegexParsers with PackratParsers {

  lazy val ident: PackratParser[String] = "[A-Za-z0-9_]+".r ^^ {
    case s => s
  }

  lazy val typename: PackratParser[String] = "[A-Za-z0-9_]+".r ^^ {
    case s => s
  }

  lazy val typed: PackratParser[Typed] = typename ~ ident ^^ {
    case a ~ b => Typed(a, b)
  }

  lazy val param: PackratParser[Typed] = typed <~ ";" ^^ {
    case a => a
  }

  lazy val param_list: PackratParser[List[Typed]] = repsep(typed, ",")

  lazy val method: PackratParser[Method] = typed ~ ('(' ~> param_list <~ ')') ~ matching ^^ {
    case t ~ ls ~ src => Method(t, ls, src)
  }

  lazy val bracefree: PackratParser[String] = """[^{}]+""".r ^^ {
    case a => a
  }

  lazy val matching: PackratParser[String] = (
      "{" ~ rep(bracefree | matching) ~ "}") ^^ {
    case a ~ b ~ c => a + b.mkString("") + c
  }

  lazy val features: PackratParser[List[Param]] = 
    rep(param | method)

  lazy val CClass: PackratParser[Class] =
      ("class" ~> ident <~ ":") ~ ("{:public" ~> features <~ "}") ~ ("{:private" ~> features <~ "}") ^^ {
    case nm ~ pub ~ priv => Class(nm, pub, priv)
  }
}

object Uncool extends Comp {
  def main(args: Array[String])= {
//  val sauce = io.Source.stdin.mkString
    println(parseAll(CClass, "class Car: {:public int foo; int bar(int z, int s) {} } {:private int baz(int n) {}}").get)
  }
}
