**User stories**

### **Conta de usuário**

**US01 — Cadastro**

Descrição: Como visitante, quero me cadastrar com nome, email e senha, para criar minha conta no sistema.

**Critérios de aceitação:**

* O sistema exige nome, email e senha para completar o cadastro.  
* O sistema valida que o email tem formato válido e ainda não está cadastrado.  
* A senha deve atender a um critério mínimo de segurança (ex: tamanho mínimo).  
* Após cadastro bem-sucedido, usuário é logado automaticamente ou redirecionado para o login.

**US02 — Login**

Descrição: Como usuário cadastrado, quero fazer login, para acessar meus clubes e meu perfil. 

**Critérios de aceitação:**

* Sistema permite login com email e senha cadastrados.  
* O sistema exibe mensagem de erro clara quando as credenciais estão incorretas.  
* Após login, o usuário é direcionado à sua tela inicial (feed/perfil).  
* O usuário permanece logado até logout manual ou expiração de sessão.

**US27 — Personalização de perfil**

Descrição: Como usuário, quero poder personalizar meu perfil, incluindo foto, header e biografia.

**Critérios de aceitação:**

* O usuário pode adicionar/atualizar foto de perfil.  
* O usuário pode adicionar/atualizar imagem de capa (header).  
* O usuário pode escrever uma biografia com limite de caracteres definido.  
* Alterações no perfil são refletidas imediatamente para quem visualiza o perfil.  
* Todos os campos de personalização são opcionais.

### **Clubes e membros**

**US03 — Criar clube**

Descrição: Como usuário, quero criar um clube do livro e me tornar automaticamente seu administrador, para organizar a leitura em grupo.

**Critérios de aceitação:**

* Qualquer usuário cadastrado pode criar um clube informando ao menos um nome.  
* O criador do clube é definido automaticamente como administrador.  
* O sistema exige a definição do tipo do clube (ditatorial/democrático) na criação.  
* Clube criado aparece imediatamente na lista de clubes do usuário.

**US04 — Definir tipo do clube**

Descrição: Como usuário, quero definir se meu clube é ditatorial ou democrático no momento da criação, para que as regras de decisão fiquem claras desde o início.

**Critérios de aceitação:**

* O sistema exige escolha entre "ditatorial" e "democrático" na criação.  
* Tipo do clube não pode ser alterado depois de criado.  
* Se democrático, o sistema também solicita o método de seleção de livro (votação ou sorteio).  
* Interface explica o que cada tipo significa antes da confirmação.

**US05 — Gerar convite**

Descrição: Como administrador, quero gerar um link de convite para o clube, para que outras pessoas possam entrar.

**Critérios de aceitação:**

* O sistema gera um link único de convite por clube, acessível apenas pelo administrador.  
* Qualquer pessoa com o link pode solicitar entrada no clube.  
* O administrador pode visualizar/copiar o link a qualquer momento.  
* O administrador pode revogar e gerar um novo link.

**US06 — Entrar via convite**

Descrição: Como usuário convidado, quero entrar em um clube por meio do link de convite, para passar a fazer parte dele.

**Critérios de aceitação:**

* Ao acessar um link válido, o usuário vê informações básicas do clube antes de confirmar a entrada.  
* Usuário é adicionado como membro após confirmação.  
* O sistema impede que o mesmo usuário entre duas vezes no mesmo clube.  
* Se não estiver logado, o sistema solicita login/cadastro antes de processar o convite.

**US07 — Participar de vários clubes**

Descrição: Como usuário, quero participar de vários clubes ao mesmo tempo, para acompanhar diferentes grupos de leitura.

**Critérios de aceitação:**

* Usuário pode ser membro de múltiplos clubes simultaneamente.  
* Lista de clubes do usuário exibe todos os clubes que ele participa.  
* Perfil agrega check-ins de todos os clubes do usuário.  
* Trocar de clube não exige novo login.

**US08 — Expulsar membro**

Descrição: Como administrador, quero expulsar um membro do clube, para remover alguém que não deveria mais participar.

**Critérios de aceitação:**

* Apenas o administrador tem acesso à ação de expulsar.  
* Membro expulso perde acesso ao feed e conteúdo do clube imediatamente.  
* Sistema solicita confirmação antes de efetivar a expulsão.  
* Check-ins já publicados pelo membro expulso permanecem no histórico do clube.

**US28 — Sair do clube**

Descrição: Como usuário, quero poder sair de um clube do qual participo.

**Critérios de aceitação:**

* Membros comuns têm uma opção explícita de "sair do clube"; o administrador não pode sair.  
* Sistema solicita confirmação antes de efetivar a saída.  
* Após sair, o usuário perde acesso ao feed, catálogo e conteúdo do clube.  
* Check-ins publicados anteriormente permanecem no histórico do clube.

### **Catálogo / biblioteca do clube**

**US09 — Sugerir livro (democrático)**

Descrição: Como membro de um clube democrático, quero sugerir um livro para o catálogo, para que ele seja considerado como a próxima leitura.

**Critérios de aceitação:**

* Qualquer membro de um clube democrático pode adicionar um livro ao catálogo com status "quero ler".  
* Livro sugerido deve conter ao menos título e autor.  
* Livro sugerido fica visível para todos os membros do clube.  
* Essa ação não é permitida para membros de clubes ditatoriais.

**US10 — Adicionar livro (ditatorial)**

Descrição: Como administrador de um clube ditatorial, quero ser o único a adicionar livros ao catálogo, para manter o controle da escolha das leituras.

**Critérios de aceitação:**

* Em clube ditatorial, apenas o administrador pode adicionar livros ao catálogo.  
* A opção de adicionar livro fica oculta/bloqueada para membros comuns nesse tipo de clube.  
* Livro adicionado entra no catálogo com status "quero ler".  
* Tentativa de um membro comum de adicionar livro é rejeitada com mensagem explicativa.

**US11 — Escolher método de seleção**

Descrição: Como administrador, quero escolher, ao criar um clube democrático, se a seleção do próximo livro será por votação ou sorteio, para definir a regra de decisão do grupo.

**Critérios de aceitação:**

* Na criação de um clube democrático, o sistema solicita a escolha entre "votação" e "sorteio".  
* Escolha fica registrada como configuração do clube.  
* O sistema aplica o método escolhido sempre que for necessário selecionar o próximo livro.  
* A tela do clube exibe qual método está configurado.

**US12 — Votar em livro**

Descrição: Como membro de um clube democrático com votação ativada, quero votar entre os livros sugeridos, para que o mais desejado pelo grupo seja escolhido.

**Critérios de aceitação:**

* Cada membro vota em apenas um livro sugerido por rodada de votação.  
* O sistema impede que um membro vote mais de uma vez na mesma rodada.  
* Livro com mais votos ao final da rodada se torna automaticamente o livro atual ("lendo").  
* Em caso de empate, o sistema aplica uma regra de desempate definida (ex: sorteio entre os empatados).

**US13 — Sorteio de livro**

Descrição: Como membro de um clube democrático com sorteio ativado, quero que o sistema sorteie um livro entre os sugeridos, para que a escolha seja aleatória e justa.

**Critérios de aceitação:**

* Cada membro escolhe um livro com status "quero ler" e esses são elegíveis para o sorteio.  
* Sorteio escolhe um único livro de forma aleatória entre os elegíveis.  
* Livro sorteado muda automaticamente para status "lendo" e vira o livro atual.  
* Sistema exibe aos membros qual livro foi sorteado.

**US14 — Escolher livro (ditatorial)**

Descrição: Como administrador de um clube ditatorial, quero escolher diretamente qual livro do catálogo vira a leitura atual, para decidir sozinho o rumo do clube.

**Critérios de aceitação:**

* Apenas o administrador pode selecionar o próximo livro atual entre os do catálogo.  
* Livro escolhido muda para status "lendo" e vira o livro atual do clube.  
* Membros comuns não têm acesso a essa ação.  
* Apenas um livro pode estar como "lendo" por vez em cada clube.

**US15 — Ver catálogo por status**

Descrição: Como membro, quero ver os livros do catálogo separados por status (quero ler / lendo / já lido), para entender em que fase cada livro está.

**Critérios de aceitação:**

* A tela do catálogo exibe os livros agrupados/filtrados por status.  
* Existe no máximo um livro com status "lendo" em destaque.  
* Livros "já lidos" ficam acessíveis em uma seção separada.  
* Mudança de status de um livro reflete automaticamente na visualização.

**US16 — Estatísticas de livros lidos**

Descrição: Como membro, quero ver estatísticas dos livros já lidos pelo clube (nota média, resenhas), para relembrar as leituras anteriores do grupo.

**Critérios de aceitação:**

* Para cada livro "já lido", o sistema exibe a nota média das avaliações dos membros.  
* O sistema exibe as resenhas/comentários deixados pelos membros sobre o livro.  
* Estatísticas são recalculadas conforme novas avaliações são adicionadas.  
* Se nenhum membro avaliar o livro, o sistema indica "sem avaliações" em vez de erro ou nota zerada.

### **Meta de leitura**

**US17 — Definir prazo**

Descrição: Como administrador, quero definir um prazo para que os membros terminem o livro atual, para que o clube tenha um ritmo de leitura combinado.

**Critérios de aceitação:**

* O administrador pode definir uma data-limite associada ao livro atual do clube.  
* Prazo definido é visível para todos os membros.  
* O administrador pode alterar o prazo enquanto o livro estiver "lendo".  
* Prazo é o mesmo para todos os membros do clube (não individual).

### **Check-in**

**US18 — Registrar check-in**

Descrição: Como membro, quero registrar um check-in com a página em que estou e um comentário breve, para compartilhar meu progresso com o clube.

**Critérios de aceitação:**

* O sistema exige o preenchimento da página atual e de um comentário para publicar.  
* Check-in publicado é associado automaticamente ao livro atual do clube e ao autor.  
* Check-in aparece imediatamente no feed do clube após publicação.  
* Página informada deve ser um número válido (maior que zero).

**US19 — Reação no check-in**

Descrição: Como membro, quero adicionar uma reação opcional ao meu check-in, para expressar rapidamente o que achei da leitura.

**Critérios de aceitação:**

* Reação é um campo opcional; check-in pode ser publicado sem ela.  
* O sistema oferece um conjunto definido de reações para escolha.  
* A reação escolhida fica visível junto ao check-in no feed.

**US20 — Marcar livro terminado**

Descrição: Como membro, quero que, ao finalizar uma leitura (atingir 100% de páginas lidas)o meu progresso seja atualizado, indicando que terminei o livro para eu ficar em destaque no ranking (cor diferente) pro resto do clube.

**Critérios de aceitação:**

* O sistema calcula automaticamente que o usuário terminou o livro quando chega em 100% das páginas lidas.   
* O membro que termina o livro tem seu nome/avatar destacado com cor diferente na lista de membros/feed.  
* Marcação vale apenas para o livro atual; ao trocar de livro, o destaque é reiniciado para todos.  
* É possível identificar, num único lugar (ranking do clube), quais membros já terminaram o livro atual e qual o progresso dos outros membros também. 

**US21 — Apagar check-in**

Descrição: Como membro, quero apagar um check-in que publiquei, para remover algo postado por engano.

**Critérios de aceitação:**

* Apenas o autor do check-in pode apagá-lo.  
* Sistema solicita confirmação antes de apagar.  
* Check-in apagado desaparece do feed do clube e do perfil do usuário.  
* Comentários/reações associados ao check-in apagado também são removidos.

### **Feed e perfil**

**US22 — Feed do clube**

Descrição: Como membro, quero ver o feed do clube com os check-ins de todos em ordem cronológica, agrupados por dia, para acompanhar o progresso do grupo.

**Critérios de aceitação:**

* Feed exibe check-ins em ordem cronológica decrescente.  
* Check-ins ficam agrupados visualmente por dia de publicação.  
* Feed atualiza (ou permite atualizar) quando novos check-ins são publicados.  
* Apenas membros do clube conseguem visualizar o feed.

**US23 — Histórico no perfil**

Descrição: Como usuário, quero ver no meu perfil os check-ins de todos os clubes que participo, para ter um histórico pessoal da minha leitura.

**Critérios de aceitação:**

* Perfil lista todos os check-ins feitos pelo usuário, independente do clube.  
* Cada check-in exibido indica a qual clube e livro pertence.  
* Lista segue ordem cronológica decrescente.  
* Check-ins apagados não aparecem no perfil.

**US24 — Privacidade do clube**

Descrição: Como usuário, quero que meu clube seja visível apenas para seus membros, para manter a privacidade do grupo.

**Critérios de aceitação:**

* Usuários não-membros não conseguem acessar feed, catálogo ou lista de membros do clube.  
* Acesso não autorizado resulta em mensagem de acesso negado ou redirecionamento.  
* Apenas convite/link permite a um novo usuário visualizar o conteúdo (após entrar).  
* O Clube não aparece em nenhuma busca/listagem pública do sistema.

### **Comentários e reações em check-ins**

**US25 — Comentar em check-in**

Descrição: Como membro, quero comentar no check-in de outro membro do meu clube, para interagir sobre o progresso dele.

**Critérios de aceitação:**

* Apenas membros do mesmo clube podem comentar em um check-in.  
* O comentário respeitará	 um limite máximo de caracteres (menor que um tweet).  
* O comentário publicado aparece imediatamente associado ao check-in, com autor e data/hora.  
* É possível haver múltiplos comentários de diferentes membros no mesmo check-in.

**US26 — Reagir a check-in**

Descrição: Como membro, quero reagir ao check-in de outro membro sem precisar escrever um comentário, para demonstrar apoio de forma rápida.

**Critérios de aceitação:**

* Reagir é uma ação independente de comentar (não exige comentário).  
* Apenas membros do mesmo clube podem reagir.  
* O sistema exibe a quantidade de reações recebidas por um check-in.  
* Membro pode alterar ou remover sua própria reação.

### **Progresso e ranking**

**US29 — Ranking de progresso**

Descrição: Como membro, quero poder visualizar o ranking do progresso de cada membro na leitura atual, para estimular a leitura.

**Critérios de aceitação:**

* O sistema exibe uma lista dos membros do clube com o progresso (ex: página atual) na leitura do livro atual.  
* A lista destaca visualmente quem já terminou o livro atual (mesmo destaque de cor da US20).  
* O ranking é calculado com base no check-in mais recente de cada membro para o livro atual.  
* O ranking é visível para todos os membros, não só para o administrador.

