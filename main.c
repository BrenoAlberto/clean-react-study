#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Cliente
struct Telefone
{
    int ddd;
    int numero;
};
typedef struct Telefone telefoneEntity;

struct Endereco
{
    int numero;
    char rua[20],complemento[15],bairro[20],cidade[20],uf[3],cep[10];
};
typedef struct Endereco enderecoEntity;

struct Cliente
{
    int codigo;
    telefoneEntity telefone;
    enderecoEntity endereco;
    char nome[30];
};
typedef struct Cliente clienteEntity;

//Locação
struct Data
{
    int dia,mes,ano;
};
typedef struct Data dataEntity;

struct Locacao
{
    int codigo,seguro,dias,cod_c,cod_v;
    dataEntity dataRetirada;
    dataEntity dataDevolucaoPret;
    dataEntity dataDevolucao;
};
typedef struct Locacao locacaoEntity;

//Veiculo
struct Veiculo
{
    int codigo,qnt,status;
    char descricao[100],modelo[30],cor[30],placa[9];
    float valor;
};
typedef struct Veiculo veiculoEntity;


FILE *arquivoClientes;
FILE *arquivoVeiculos;
FILE *arquivoLocacao;
int main() {

    setlocale(LC_ALL,"portuguese");

    inicializaArquivos(arquivoClientes, arquivoVeiculos, arquivoLocacao);


    int menu;
    do {
        printf("\n\n============================\n");
        printf("Selecione uma opção:\n");
        printf("1 - Cadastrar um cliente\n");
        printf("2 - Cadastrar Veículo\n");
        printf("3 - Cadastrar Locação\n");
        printf("4 - Baixa em locação\n");
        printf("5 - Localizar dados\n");
        printf("6 - Locação por cliente\n");
        printf("7 - Pontuação do cliente\n");
        printf("8 - Clientes que recebem kit\n");
        printf("9 - Exibe locações atrasadas\n");
        printf("0 - Sair do programa\n");
        printf("Opcao: ");
        scanf("%d",&menu);
        printf("============================");
        switch(menu)
        {
            case 0:
                printf("Saindo... \n");
                break;
            case 1:
                cadastraCliente(arquivoClientes);
                break;
            case 2:
                cadastraVeiculo(arquivoVeiculos);
                break;
            case 3:
                cadastraLocacao(arquivoLocacao, arquivoClientes, arquivoVeiculos);
                break;
            case 4:
                baixaLocacao(arquivoLocacao, arquivoClientes, arquivoVeiculos);
                break;
            case 5:
                localizaDados(arquivoClientes, arquivoVeiculos);
                break;
            case 6:
                buscaLocacoesCliente(arquivoClientes, arquivoLocacao);
                break;
            case 7:
                imprimePonto(arquivoClientes, arquivoLocacao);
                break;
            case 8:
                atingiuKit(arquivoClientes, arquivoLocacao);
                break;
            case 9:
                imprimeLocacoesAtrasadas(arquivoLocacao);
                break;
            default:
                printf("Opção inválida.\n\n");
                break;
        }
    }while(menu != 0);

}

/*BEAN CLIENTE*/
void cadastraCliente(FILE *arquivoClientes)
{
    printf("\n====Cadastra Cliente====\n");
    clienteEntity c;
    int posicao,codigo;
    printf("\nCódigo:             ");
    scanf("%d",&codigo);
    posicao=encontraCliente(arquivoClientes,codigo,"---");
    if (posicao==-1)
    {
        c.codigo=codigo;
        printf("Nome:               ");
        fflush(stdin);
        gets(c.nome);

        cadastraTelefone(&c.telefone);

        cadastraEndereco(&c.endereco);

        fseek(arquivoClientes,0,SEEK_END);
        fwrite(&c, sizeof(c),1,arquivoClientes);
        fflush(arquivoClientes);

        printf("\nCliente cadastrado com sucesso!");
    }
    else
    {
        printf("Já existe um cliente com esse código. Inclusão não realizada!\n");
    }
}

int encontraCliente(FILE *arquivoClientes,int codigo, char nome[30])
{
    int posicao=-1,achou=0;
    clienteEntity c;
    fseek(arquivoClientes,0,SEEK_SET);
    fread(&c, sizeof(c),1, arquivoClientes);
    while (!feof(arquivoClientes) && !achou)
    {
        posicao++;
        if (c.codigo==codigo || strcmpi(c.nome,nome)==0)
        {
            achou=1;
        }
        fread(&c, sizeof(c),1, arquivoClientes);
    }
    if (achou)
    {
        return posicao;
    }
    else
    {
        return -1;
    }
}

void todosClientes(FILE *arquivoClientes){
    clienteEntity c;
    fseek(arquivoClientes,0,SEEK_SET);
    fread(&c, sizeof(c),1, arquivoClientes);
    printf("\n====Imprimindo todos clientes====\n");
    while (!feof(arquivoClientes))
    {
        imprimeCliente(c);
        fread(&c, sizeof(c),1, arquivoClientes);
    }
    printf("==================================\n");
}

void imprimeCliente(clienteEntity c){
    printf("\n\nCodigo:             %i\n", c.codigo);
    printf("Nome:               %s\n", c.nome);
    printf("Telefone:           (%i) %i\n", c.telefone.ddd, c.telefone.numero);
    printf("Endereco:           %i, %s, %s, %s, %s, %s, %s\n", c.endereco.numero, c.endereco.rua, c.endereco.complemento, c.endereco.bairro, c.endereco.cidade, c.endereco.cep, c.endereco.uf);
}

void cadastraTelefone(telefoneEntity *obj){
    telefoneEntity t;

    printf("\n=======Digite o telefone=======\n");
    printf("DDD do telefone:    ");
    fflush(stdin);
    scanf("%d", &t.ddd);
    printf("Número do telefone: ");
    fflush(stdin);
    scanf("%i",&t.numero);

    *obj = t;
}

void cadastraEndereco(enderecoEntity *obj){
    enderecoEntity e;

    printf("\n=======Digite o endere�o do cliente=======\n");
    printf("Rua:                ");
    fflush(stdin);
    gets(e.rua);
    printf("Número:             ");
    fflush(stdin);
    scanf("%d",&e.numero);
    printf("Complemento:        ");
    fflush(stdin);
    gets(e.complemento);
    printf("Bairro:             ");
    fflush(stdin);
    gets(e.bairro);
    printf("Cidade:             ");
    fflush(stdin);
    gets(e.cidade);
    printf("UF:                 ");
    fflush(stdin);
    gets(e.uf);
    printf("CEP:                ");
    fflush(stdin);
    gets(e.cep);

    *obj = e;
}

/*BEAN VEÍCULO*/
void cadastraVeiculo(FILE *arquivoVeiculos)
{
    printf("\n====Cadastra Veículo====\n");
    veiculoEntity v;
    int posicao,codigo;
    printf("Digite o c�digo do ve�culo...:");
    scanf("%d",&codigo);
    posicao=encontraVeiculo(arquivoVeiculos,codigo,0);
    if (posicao==-1)
    {
        v.codigo=codigo;
        printf("Digite o modelo do veículo:                 ");
        fflush(stdin);
        gets(v.modelo);
        printf("Digite a descrição do veículo:              ");
        fflush(stdin);
        gets(v.descricao);
        printf("Digite a cor do veículo:                    ");
        fflush(stdin);
        gets(v.cor);
        printf("Digite a placa do veículo:                  ");
        fflush(stdin);
        scanf("%s",v.placa);
        printf("Digite o valor da diária do veículo:        ");
        fflush(stdin);
        scanf("%f",&v.valor);
        printf("Digite a quantidade de ocupantes do veículo:");
        fflush(stdin);
        scanf("%d",&v.qnt);
        v.status=0;
        fseek(arquivoVeiculos,0,SEEK_END);
        fwrite(&v, sizeof(v),1,arquivoVeiculos);
        fflush(arquivoVeiculos);
        printf("\nVeiculo cadastrado com sucesso!");
    }
    else
    {
        printf("Já existe um veículo com este código. Inclusão não realizada!\n");
    }
}

int encontraVeiculo(FILE *arquivoVeiculos,int codigo,int qnt){
    int posicao=-1,achou=0;
    veiculoEntity v;
    fseek(arquivoVeiculos,0,SEEK_SET);
    fread(&v, sizeof(v),1, arquivoVeiculos);
    while (!feof(arquivoVeiculos) && !achou)
    {
        posicao++;
        if (v.codigo==codigo || ((v.qnt==qnt && v.status==0) || qnt == -1))
        {
            achou=1;
        }
        fread(&v, sizeof(v),1, arquivoVeiculos);
    }
    if (achou)
    {
        return posicao;
    }
    else
    {
        return -1;
    }
}

void encontraTodosVeiculos(FILE *arquivoVeiculos){
    veiculoEntity v;
    fseek(arquivoVeiculos,0,SEEK_SET);
    fread(&v, sizeof(v),1, arquivoVeiculos);
    printf("\n=====Todos veículos=======\n");
    while (!feof(arquivoVeiculos))
    {
        imprimeVeiculo(v);
        fread(&v, sizeof(v),1, arquivoVeiculos);
    }
    printf("================================\n");
}

void imprimeVeiculo(veiculoEntity v){
    printf("\n\nCodigo:             %i\n", v.codigo);
    printf("qnt passageiros:    %i\n", v.qnt);
    printf("Status:             %i\n", v.status);
    printf("Descricao:          %s\n", v.descricao);
    printf("Modelo:             %s\n", v.modelo);
    printf("Cor:                %s\n", v.cor);
    printf("Placa:              %s\n", v.placa);
    printf("Valor diária:       %f\n", v.valor);
}

/*BEAN LOCACAO*/
void cadastraLocacao(FILE *arquivoLocacao, FILE *arquivoClientess, FILE *arquivoVeiculos)
{
    printf("\n====Cadastra Locação====\n");
    locacaoEntity l;
    clienteEntity c;
    veiculoEntity v;
    int pLocacao,pVeiculo,pCliente,qtd,codigo,codigoC;
    printf("\nDigite o código da locação:   ");
    scanf("%d",&codigo);

    pLocacao=encontraLocacao(arquivoLocacao,codigo);
    if (pLocacao==-1)
    {
        l.codigo=codigo;
        printf("Digite o código do cliente:");
        fflush(stdin);
        scanf("%i", &codigoC);
        pCliente=encontraCliente(arquivoClientess,codigoC,"---");
        if (pCliente!=-1)
        {

            fseek(arquivoClientess,sizeof(c)*pCliente,SEEK_SET);
            fread(&c,sizeof(c),1,arquivoClientess);
            fflush(stdin);

            int continuar =0;
            printf("Cliente selecionado: %s\nDigite 1 para continuar e 0 para retornar:", c.nome );
            fflush(stdin);
            scanf("%i", &continuar);

            if(continuar == 0){ //sair do método
                return;
            }

            l.cod_c=c.codigo;
            printf("\nDigite o dia, mês e ano da retirada do veículo:     ");
            fflush(stdin);
            scanf("%d%d%d",&l.dataRetirada.dia,&l.dataRetirada.mes,&l.dataRetirada.ano);
            printf("Digite o dia, mês e ano da devolução do veículo:    ");
            fflush(stdin);
            scanf("%d%d%d",&l.dataDevolucaoPret.dia,&l.dataDevolucaoPret.mes,&l.dataDevolucaoPret.ano);
            printf("Digite a quantidade de ocupantes do veiculo:        ");
            fflush(stdin);
            scanf("%d",&qtd);
            fflush(stdin);
            l.dataDevolucao.ano = 0;
            l.dataDevolucao.dia = 0;
            l.dataDevolucao.mes = 0;
            l.dias=calculaDias(l.dataRetirada.dia,l.dataRetirada.mes,l.dataRetirada.ano,l.dataDevolucaoPret.dia,l.dataDevolucaoPret.mes,l.dataDevolucaoPret.ano);
            pVeiculo=encontraVeiculo(arquivoVeiculos,-1,qtd);
            if (pVeiculo!=-1)
            {
                fseek(arquivoVeiculos,sizeof(v)*pVeiculo,SEEK_SET);
                fread(&v,sizeof(v),1,arquivoVeiculos);
                fflush(stdin);
                l.cod_v=v.codigo;
                v.status=1;

                imprimeVeiculo(v);

                printf("Seguro? (0 - não 1- sim): ");
                fflush(stdin);
                scanf("%d",&l.seguro);

                fseek(arquivoVeiculos,sizeof(v)*pVeiculo,SEEK_SET);
                fwrite(&v, sizeof(v),1,arquivoVeiculos);
                fflush(arquivoVeiculos);

                fseek(arquivoLocacao,sizeof(l)*pLocacao,SEEK_SET);
                fwrite(&l, sizeof(l),1,arquivoLocacao);
                fflush(arquivoLocacao);
                printf("\nLocação cadastrada com sucesso!");
            }
            else
            {
                printf("Não há carros disponíveis para esta quantidade de passageiros!\n");
            }

        }
        else
        {
            printf("Não há clientes com esse nome. Por favor, cadastrar o cliente antes de realizar a locação.\n");
        }

    }
    else
    {
        printf("Já existe uma locação com esse código. Inclusão não realizada!\n");
    }
}

int encontraLocacao(FILE *arquivoLocacao,int codigo)
{
    int posicao=-1,achou=0;
    locacaoEntity l;
    fseek(arquivoLocacao,0,SEEK_SET);
    fread(&l, sizeof(l),1, arquivoLocacao);
    while (!feof(arquivoLocacao) && !achou)
    {
        posicao++;
        if (l.codigo==codigo)
        {
            achou=1;
        }
        fread(&l, sizeof(l),1, arquivoLocacao);
    }
    if (achou)
    {
        return posicao;
    }
    else
    {
        return -1;
    }
}

void baixaLocacao(FILE *arquivoLocacao, FILE *arquivoClientes, FILE *arquivoVeiculos)
{
    printf("\n====Baixa Locação====\n");
    locacaoEntity l;
    clienteEntity c;
    veiculoEntity v;
    int posicao1,pVeiculo,posicao3,codigo;
    float multa,valortotal;
    printf("Digite o c�digo da loca��o...:");
    scanf("%d",&codigo);
    posicao1=encontraLocacao(arquivoLocacao,codigo);

    if (posicao1!=-1)
    {

        fseek(arquivoLocacao,sizeof(l)*(posicao1),SEEK_SET);
        fread(&l, sizeof(l),1, arquivoLocacao);
        printf("Digite o dia, mês e ano da entrega do veículo:");
        fflush(stdin);
        scanf("%d%d%d",&l.dataDevolucao.dia,&l.dataDevolucao.mes,&l.dataDevolucao.ano);

        //Calcula dias alugado e dias que era p ter sido
        int diasPretencaoAlugado = calculaDias(l.dataRetirada.dia,l.dataRetirada.mes,l.dataRetirada.ano,    l.dataDevolucaoPret.dia,l.dataDevolucaoPret.mes,l.dataDevolucaoPret.ano);
        int diasAlugado = calculaDias(l.dataRetirada.dia,l.dataRetirada.mes,l.dataRetirada.ano,    l.dataDevolucao.dia,l.dataDevolucao.mes,l.dataDevolucao.ano);

        pVeiculo = encontraVeiculo(arquivoVeiculos,l.cod_v,0);

        if (pVeiculo!=-1)
        {

            //Faz alterações nos arquivos
            fseek(arquivoVeiculos,sizeof(v)*(pVeiculo),SEEK_SET);
            fread(&v, sizeof(v),1, arquivoVeiculos);

            fflush(stdin);
            v.status=0;

            fseek(arquivoVeiculos,sizeof(v)*pVeiculo,SEEK_SET);
            fwrite(&v, sizeof(v),1,arquivoVeiculos);
            fflush(arquivoVeiculos);

            fseek(arquivoLocacao,sizeof(l)*posicao1,SEEK_SET);
            fwrite(&l, sizeof(l),1,arquivoLocacao);
            fflush(arquivoLocacao);


            //Calculo de valor total
            valortotal=(diasAlugado*v.valor);
            printf("Valor total das di�rias.:%.2f\n",valortotal);
            if(diasAlugado-diasPretencaoAlugado > 0)
                multa=valortotal*0.05+30*(diasAlugado-diasPretencaoAlugado);
            if(multa < 0){
                multa = 0;
            }
            printf("Multa por dias de atraso:%.2f\n",multa);
            if(l.seguro==1)
            {
                valortotal=valortotal+multa+50.00;
                printf("\nSeguro contratado. Adicional de R$50.00!\n");
                printf("Valor do seguro         :R$50.00\n");
                printf("\n\nValor total da locação  :R$%.2f\n",valortotal);
            }
            else
            {
                valortotal=valortotal+multa;
                printf("\n\nValor total da locação  :R$\n",valortotal);
            }

            //Impressão de qual cliente deverá arcar com o valor
            posicao3=encontraCliente(arquivoClientes,l.cod_c,"---");
            if (posicao3!=-1)
            {
                fseek(arquivoClientes,sizeof(c)*(posicao3),SEEK_SET);
                fread(&c, sizeof(c),1, arquivoClientes);
                printf("\nC�digo do cliente      :%d\n",c.codigo);
                printf("Nome do cliente         :%s\n",c.nome);
            }
            printf("\nBaixa realizada com sucesso!");
        }
        else{
            printf("Veículo não encontrado! \n");
        }
    }
    else
    {
        printf("Locação não encontrada! \n");
    }


}

void imprimeLocacao(locacaoEntity l) {
    printf("\n\nCódigo:                 %d \n",l.codigo);
    printf("Data da Retirada:           %d/%d/%d \n",l.dataRetirada.dia,l.dataRetirada.mes,l.dataRetirada.ano);
    printf("Data da Devolução:          %d/%d/%d\n",l.dataDevolucaoPret.dia,l.dataDevolucaoPret.mes,l.dataDevolucaoPret.ano);
    printf("Data da Entrega:            %d/%d/%d\n",l.dataDevolucao.dia,l.dataDevolucao.mes,l.dataDevolucao.ano);
    printf("Dias de Aluguel:            %d\n",l.dias);
    printf("Seguro?                     %d\n",l.seguro);
    printf("Código do Cliente:          %d\n",l.cod_c);
    printf("Código do Veículo:          %d\n",l.cod_v);
}

void localizaDados(FILE *arquivoClientes, FILE *arquivoVeiculos) {
    printf("\n====Localização de dados====\n");
    printf("Digite: \n");
    printf("1 - Localizar cliente\n");
    printf("2 - Imprimir todos clientes\n");
    printf("3 - Localizar veículo\n");
    printf("4 - Imprimir todos veículos\n");
    printf("Insira: ");
    int opt;
    scanf("%i", &opt);
    switch(opt){
        case 1:
            printf("Insira o código: ");
            int codigo;
            scanf("%i", &codigo);
            int pCliente = encontraCliente(arquivoClientes, codigo, "---");
            if (pCliente!=-1)
            {
                clienteEntity c;
                fseek(arquivoClientes,sizeof(c)*pCliente,SEEK_SET);
                fread(&c,sizeof(c),1,arquivoClientes);
                fflush(stdin);
                
                imprimeCliente(c);
            } else {
                printf("\nCliente não encontrado!\n");
            }
            break;
        case 2:
            todosClientes(arquivoClientes);
            break;
        case 3:
            printf("Insira o código: ");
            int codigoV = 0;
            scanf("%i", &codigoV);
            int pVeiculo = encontraVeiculo(arquivoVeiculos, codigoV, 0);
            if (pVeiculo!=-1)
            {
                veiculoEntity v;
                fseek(arquivoVeiculos,sizeof(v)*pVeiculo,SEEK_SET);
                fread(&v,sizeof(v),1,arquivoVeiculos);
                fflush(stdin);
                
                imprimeVeiculo(v);
            } else {
                printf("\nVeículo não encontrado!\n");
            }
            break;
        case 4:
            encontraTodosVeiculos(arquivoVeiculos);
            break;
        default:
            printf("\nOpção inválida!\n");

    }
}

void buscaLocacoesCliente(FILE *arquivoClientes,FILE *arquivoLocacao)
{
    clienteEntity c;
    locacaoEntity l;
    int codCli;
    int pCliente;

    printf("Digite o código do cliente:");
    fflush(stdin);
    scanf("%i", &codCli);

    pCliente = encontraCliente(arquivoClientes,codCli,'----');
    if(pCliente != -1){
        fseek(arquivoClientes,sizeof(c)*(pCliente),SEEK_SET);
        fread(&c, sizeof(c),1, arquivoClientes);

        fseek(arquivoLocacao,0,SEEK_SET);
        fread(&l, sizeof(l),1, arquivoLocacao);
        printf("\n============================\n");
        printf("Locações de %s:", c.nome);
        
        while (!feof(arquivoLocacao))
        {
            if (l.cod_c==c.codigo)
            {
                
                imprimeLocacao(l);
            }
            fread(&l, sizeof(l),1, arquivoLocacao);
        }
        printf("===================================\n");
    } else
        printf("Cliente não encontrado! \n");

}

void imprimePonto (FILE *arquivoClientes,FILE *arquivoLocacao)
{
    clienteEntity c;
    locacaoEntity l;
    int codCli;
    int pCliente, pontos;

    printf("\nDigite o código do cliente:");
    fflush(stdin);
    scanf("%i", &codCli);

    pCliente = encontraCliente(arquivoClientes,codCli,'----');

    fseek(arquivoClientes,sizeof(c)*(pCliente),SEEK_SET);
    fread(&c, sizeof(c),1, arquivoClientes);

    fseek(arquivoLocacao,0,SEEK_SET);
    fread(&l, sizeof(l),1, arquivoLocacao);

    while (!feof(arquivoLocacao))
    {
        if (l.cod_c==c.codigo)
        {
            int dias = calculaDias(l.dataRetirada.dia, l.dataRetirada.mes, l.dataRetirada.ano, l.dataDevolucao.dia, l.dataDevolucao.mes, l.dataDevolucao.ano);
            pontos+=dias*10;

        }
        fread(&l, sizeof(l),1, arquivoLocacao);
    }
    printf("O total de pontos de %s é:%d",c.nome,pontos);

}

void atingiuKit(FILE *arquivoClientes,FILE *arquivoLocacao)
{
    clienteEntity c;
    locacaoEntity l;
    int pontos=0;
    fseek(arquivoClientes,0,SEEK_SET);
    fread(&c, sizeof(c),1, arquivoClientes);

    fseek(arquivoLocacao,0,SEEK_SET);
    fread(&l, sizeof(l),1, arquivoLocacao);
    printf("\n===========================\n");
    printf("Clientes que receberam kit: ");
    while(!feof(arquivoClientes))
    {
        while (!feof(arquivoLocacao))
        {
            if (l.cod_c==c.codigo)
            {
                int dias = calculaDias(l.dataRetirada.dia, l.dataRetirada.mes, l.dataRetirada.ano, l.dataDevolucao.dia, l.dataDevolucao.mes, l.dataDevolucao.ano);
                pontos+=dias*10;

            }
            
            fread(&l, sizeof(l),1, arquivoLocacao);
        }
        if(pontos>=500)
        {
            printf("\nO cliente %s tem %d pontos e pode retirar um kit da LocaMais",c.nome,pontos);
        }
        pontos = 0;
        fread(&c, sizeof(c),1, arquivoClientes);
    }
    printf("\n===========================\n");
}

void imprimeLocacoesAtrasadas(FILE *arquivoLocacao){
    locacaoEntity l;
    dataEntity d;

    fseek(arquivoLocacao,0,SEEK_SET);
    fread(&l, sizeof(l),1, arquivoLocacao);

    printf("\n\nInsira a data atual: (dia mes ano): ");
    fflush(stdin);
    scanf("%d%d%d", &d.dia, &d.mes, &d.ano);
    fflush(stdin);

    printf("\n===========================\n");
    printf("Locações atrasadas: ");
    while (!feof(arquivoLocacao))
    {
        if(
            ((l.dataDevolucaoPret.dia < d.dia && (l.dataDevolucao.mes <= d.mes && l.dataDevolucao.ano <= d.ano)) ||
            (l.dataDevolucao.mes < d.mes || l.dataDevolucao.ano < d.ano)) &&
            l.dataDevolucao.dia == 0

        ){
            imprimeLocacao(l);
        }
        
        fread(&l, sizeof(l),1, arquivoLocacao);
    }
    printf("\n===========================\n");
}

int calculaDias(int dia1, int mes1, int ano1, int dia2, int mes2, int ano2){
    int i, total1=0, total2=0,res;

    for (i=2022; i<ano1; i++)
    {

        if((i%4==0 && i%100!=0)||i%400==0)
        {
            total1+=366;
        }
        else
        {
            total1+=365;
        }
    }
    for (i=1; i<mes1; i++)
    {
        if((i%2!=0 && i<=7)||(i%2==0 && i>7))
        {
            total1+=31;
        }

        if(i==2 && ((ano1%4==0 && ano1%100!=0)||i%400==0))
        {
            total1+=29;
        }
        if(i==2 && !((ano1%4==0 && ano1%100!=0)||i%400==0))
        {
            total1+=28;
        }
        if(((i%2==0 && i<=7) && i!=2)||(i%2!=0 && i>7))
        {
            total1+=30;
        }
    }

    total1+=dia1;

    for(i=2022; i<ano2; i++)
    {
        if((i%4==0 && i%100!=0)||i%400==0)
        {
            total2+=366;
        }
        else
        {
            total2+=365;
        }
    }
    for(i=1; i<mes2; i++)
    {
        if((i%2!=0 && i<=7)||(i%2==0 && i>7))
        {
            total2+=31;
        }

        if(i==2 && ((ano1%4==0 && ano1%100!=0)||i%400==0))
        {
            total2+=29;
        }
        if(i==2 && !((ano1%4==0 && ano1%100!=0)||i%400==0))
        {
            total2+=28;
        }
        if(((i%2==0 && i<=7) && i!=2)||(i%2!=0 && i>7))
        {
            total2+=30;
        }
    } 
    
    total2+=dia2;

    return total2-total1;
}



void inicializaArquivos(){ //function para inicializar todos os arquivos de registro
    if ((arquivoClientes=fopen("arquivoClientes.txt","r+b"))==NULL)
    {
        printf("Criando arquivo de clientes...\n");
        if ((arquivoClientes=fopen("arquivoClientes.txt","w+b"))==NULL)
        {
            printf("Erro na criação do arquivo de clientes!!\n");
            exit(1);
        }
    }

    if ((arquivoVeiculos=fopen("arquivoVeiculos.txt","r+b"))==NULL)
    {
        printf("Criando arquivo de veiculos...\n");
        if ((arquivoVeiculos=fopen("arquivoVeiculos.txt","w+b"))==NULL)
        {
            printf("Erro na criação do arquivo de veículos!!\n");
            exit(1);
        }
    }

    if ((arquivoLocacao=fopen("arquivoLocacao.txt","r+b"))==NULL)
    {
        printf("Criando arquivo de veiculos...\n");
        if ((arquivoLocacao=fopen("arquivoLocacao.txt","w+b"))==NULL)
        {
            printf("Erro na criação do arquivo de veículos!!\n");
            exit(1);
        }
    }

    return;
}

