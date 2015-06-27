import util.parsing.combinator._

abstract class Param {
  def toString(cls: String) : String
}
case class Typed(ty: String, nm: String) extends Param {
  override def toString() = ty + " " + nm
  def toString(cls: String) : String = ty + " " + cls + "_" + nm
}

case class Method(ty: Typed, args: List[Typed], src: String) extends Param { 
  override def toString() = ty + "(" + args.mkString(",") + ")" + src
  def toString(cls: String) : String = ty.toString(cls) + "(" + args.mkString(",") + ")" + src
}

abstract class TopLevel

case class Include(include: String) extends TopLevel {
  override def toString() = include
}

case class Class(name: String, pubs: List[Param], priv: List[Param]) extends TopLevel {
  override def toString() = {
    val header = "_" + name + "_h"
    "#ifndef " + header + "\n" +
    "#define " + header + "\n" +
    classStruct +
    allocs(header) +
    methods(header) +
    "#endif //" + header + "\n"
  }

  def allocs(cls: String) = {
    name + "Ref\n" +
    cls + "_alloc() {\n" +
      "return (" +name + "Ref) malloc(sizeof(" + name + "Ref));\n" +
    "}\n\n" +
    "void\n" +
    cls + "_dealloc(" + name + "Ref self) {\n" +
      "free(self);\n" +
    "}\n"
  }

  def classStruct : String = {
    "typedef struct _" + name + "Ref {\n" +
    pubs.filter(_.isInstanceOf[Typed]).mkString(";\n") + ";\n" +
    "} *" + name + "Ref;\n"
  }

  def methods(header: String) : String = {
    //TODO alloc
    pubs.filter(_.isInstanceOf[Method]).map(_.toString(header)).mkString("\n") + "\n" +
    priv.filter(_.isInstanceOf[Method]).map(_.toString(header)).mkString("\n") + "\n"
  }
}

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

  lazy val matching: PackratParser[String] = "{" ~> rep(bracefree | matching) <~ "}" ^^ {
    case b => "{\n" + b.mkString("") + "}"
  }

  lazy val features: PackratParser[List[Param]] = rep(param | method)

  lazy val CClass: PackratParser[Class] =
      ("class" ~> ident <~ ":") ~ ("{:public" ~> features <~ "}") ~
      ("{:private" ~> features <~ "}") ^^ {
    case nm ~ pub ~ priv => Class(nm, pub, priv)
  }

  lazy val include: PackratParser[Include] = "#[a-z]+ [^\n]+".r ^^ {
    case s => Include(s)
  }
  
  lazy val file: PackratParser[List[TopLevel]] = rep(include | CClass)

}

object Preproc extends Comp {
  def main(args: Array[String])= {
    val sauce = io.Source.stdin.mkString
    println(parseAll(file, sauce).get.mkString("\n"))
  }
}
